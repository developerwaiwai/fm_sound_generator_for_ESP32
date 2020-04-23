#include "fm_algorism.h"
#include "rom/ets_sys.h"

#define PI 3.1415926535
#define PI2 6.283185307

// サイン波形テーブル 0 - 2PI 1000点(最後1点だけは、アドレス計算でオーバーフローしないようにダミーで追加)
// MacのC言語で吐き出す
static float sin_table[1001] = {
    0.000000, 0.006283, 0.012566, 0.018848, 0.025130, 0.031411, 0.037690, 0.043968, 0.050244, 0.056519,
    0.062791, 0.069060, 0.075327, 0.081591, 0.087851, 0.094108, 0.100362, 0.106611, 0.112856, 0.119097,
    0.125333, 0.131564, 0.137790, 0.144011, 0.150226, 0.156434, 0.162637, 0.168833, 0.175023, 0.181206,
    0.187381, 0.193549, 0.199710, 0.205863, 0.212007, 0.218143, 0.224271, 0.230389, 0.236499, 0.242599,
    0.248690, 0.254771, 0.260841, 0.266902, 0.272952, 0.278991, 0.285019, 0.291036, 0.297042, 0.303035,
    0.309017, 0.314986, 0.320944, 0.326888, 0.332820, 0.338738, 0.344643, 0.350534, 0.356412, 0.362275,
    0.368125, 0.373959, 0.379779, 0.385584, 0.391374, 0.397148, 0.402907, 0.408649, 0.414376, 0.420086,
    0.425779, 0.431456, 0.437116, 0.442758, 0.448383, 0.453991, 0.459580, 0.465151, 0.470704, 0.476238,
    0.481754, 0.487250, 0.492727, 0.498185, 0.503623, 0.509041, 0.514440, 0.519817, 0.525175, 0.530511,
    0.535827, 0.541121, 0.546394, 0.551646, 0.556876, 0.562083, 0.567269, 0.572432, 0.577573, 0.582690,
    0.587785, 0.592857, 0.597905, 0.602929, 0.607930, 0.612907, 0.617859, 0.622787, 0.627691, 0.632570,
    0.637424, 0.642252, 0.647056, 0.651833, 0.656585, 0.661311, 0.666011, 0.670685, 0.675332, 0.679953,
    0.684547, 0.689113, 0.693653, 0.698165, 0.702649, 0.707106, 0.711535, 0.715936, 0.720308, 0.724653,
    0.728968, 0.733255, 0.737513, 0.741741, 0.745941, 0.750110, 0.754251, 0.758361, 0.762442, 0.766492,
    0.770513, 0.774502, 0.778462, 0.782390, 0.786288, 0.790154, 0.793990, 0.797794, 0.801566, 0.805307,
    0.809016, 0.812693, 0.816338, 0.819951, 0.823532, 0.827080, 0.830595, 0.834078, 0.837527, 0.840944,
    0.844327, 0.847677, 0.850994, 0.854277, 0.857526, 0.860741, 0.863923, 0.867070, 0.870183, 0.873262,
    0.876306, 0.879315, 0.882290, 0.885230, 0.888136, 0.891006, 0.893841, 0.896640, 0.899404, 0.902133,
    0.904826, 0.907484, 0.910105, 0.912691, 0.915240, 0.917754, 0.920231, 0.922672, 0.925076, 0.927444,
    0.929776, 0.932070, 0.934328, 0.936549, 0.938733, 0.940880, 0.942990, 0.945062, 0.947098, 0.949095,
    0.951056, 0.952979, 0.954864, 0.956711, 0.958521, 0.960293, 0.962027, 0.963723, 0.965381, 0.967001,
    0.968583, 0.970126, 0.971631, 0.973098, 0.974526, 0.975916, 0.977268, 0.978580, 0.979855, 0.981090,
    0.982287, 0.983445, 0.984564, 0.985644, 0.986685, 0.987688, 0.988651, 0.989576, 0.990461, 0.991307,
    0.992114, 0.992882, 0.993611, 0.994300, 0.994951, 0.995562, 0.996133, 0.996666, 0.997159, 0.997612,
    0.998027, 0.998401, 0.998737, 0.999033, 0.999289, 0.999506, 0.999684, 0.999822, 0.999921, 0.999980,
    1.000000, 0.999980, 0.999921, 0.999822, 0.999684, 0.999507, 0.999290, 0.999033, 0.998737, 0.998402,
    0.998027, 0.997613, 0.997159, 0.996666, 0.996134, 0.995562, 0.994951, 0.994301, 0.993612, 0.992883,
    0.992115, 0.991308, 0.990462, 0.989577, 0.988652, 0.987689, 0.986687, 0.985645, 0.984565, 0.983446,
    0.982288, 0.981091, 0.979856, 0.978582, 0.977269, 0.975918, 0.974528, 0.973099, 0.971633, 0.970128,
    0.968584, 0.967003, 0.965383, 0.963725, 0.962029, 0.960295, 0.958523, 0.956713, 0.954866, 0.952981,
    0.951058, 0.949098, 0.947100, 0.945065, 0.942992, 0.940882, 0.938735, 0.936552, 0.934331, 0.932073,
    0.929778, 0.927447, 0.925079, 0.922675, 0.920234, 0.917757, 0.915243, 0.912694, 0.910108, 0.907486,
    0.904829, 0.902136, 0.899407, 0.896643, 0.893843, 0.891008, 0.888138, 0.885233, 0.882293, 0.879318,
    0.876308, 0.873264, 0.870185, 0.867072, 0.863925, 0.860744, 0.857528, 0.854279, 0.850996, 0.847679,
    0.844329, 0.840946, 0.837529, 0.834080, 0.830597, 0.827082, 0.823534, 0.819953, 0.816340, 0.812695,
    0.809018, 0.805309, 0.801568, 0.797795, 0.793991, 0.790156, 0.786289, 0.782391, 0.778463, 0.774504,
    0.770514, 0.766493, 0.762443, 0.758362, 0.754252, 0.750111, 0.745941, 0.741742, 0.737513, 0.733255,
    0.728968, 0.724653, 0.720309, 0.715936, 0.711535, 0.707106, 0.702649, 0.698165, 0.693653, 0.689113,
    0.684546, 0.679952, 0.675332, 0.670684, 0.666011, 0.661311, 0.656584, 0.651832, 0.647054, 0.642251,
    0.637422, 0.632568, 0.627689, 0.622786, 0.617858, 0.612905, 0.607928, 0.602927, 0.597903, 0.592854,
    0.587783, 0.582688, 0.577570, 0.572429, 0.567266, 0.562080, 0.556872, 0.551643, 0.546391, 0.541118,
    0.535823, 0.530508, 0.525171, 0.519814, 0.514436, 0.509037, 0.503619, 0.498181, 0.492723, 0.487246,
    0.481749, 0.476234, 0.470699, 0.465146, 0.459575, 0.453985, 0.448378, 0.442753, 0.437110, 0.431451,
    0.425774, 0.420080, 0.414370, 0.408643, 0.402900, 0.397142, 0.391367, 0.385578, 0.379773, 0.373953,
    0.368118, 0.362269, 0.356405, 0.350527, 0.344636, 0.338731, 0.332812, 0.326881, 0.320936, 0.314979,
    0.309009, 0.303027, 0.297034, 0.291028, 0.285011, 0.278983, 0.272944, 0.266894, 0.260833, 0.254762,
    0.248681, 0.242590, 0.236490, 0.230380, 0.224262, 0.218134, 0.211998, 0.205853, 0.199700, 0.193540,
    0.187371, 0.181196, 0.175013, 0.168823, 0.162627, 0.156424, 0.150215, 0.144000, 0.137780, 0.131554,
    0.125322, 0.119086, 0.112845, 0.106600, 0.100350, 0.094097, 0.087840, 0.081579, 0.075315, 0.069048,
    0.062779, 0.056506, 0.050232, 0.043956, 0.037678, 0.031398, 0.025118, 0.018836, 0.012553, 0.006270,
    -0.000013, -0.006296, -0.012579, -0.018862, -0.025143, -0.031424, -0.037704, -0.043982, -0.050258, -0.056532,
    -0.062804, -0.069074, -0.075341, -0.081605, -0.087865, -0.094123, -0.100376, -0.106626, -0.112871, -0.119112,
    -0.125348, -0.131579, -0.137805, -0.144026, -0.150241, -0.156450, -0.162652, -0.168849, -0.175039, -0.181221,
    -0.187397, -0.193565, -0.199726, -0.205878, -0.212023, -0.218159, -0.224287, -0.230406, -0.236515, -0.242616,
    -0.248706, -0.254787, -0.260858, -0.266919, -0.272969, -0.279008, -0.285036, -0.291053, -0.297058, -0.303052,
    -0.309034, -0.315004, -0.320961, -0.326905, -0.332837, -0.338755, -0.344660, -0.350552, -0.356429, -0.362293,
    -0.368142, -0.373977, -0.379797, -0.385602, -0.391391, -0.397166, -0.402924, -0.408667, -0.414393, -0.420104,
    -0.425797, -0.431474, -0.437134, -0.442776, -0.448401, -0.454009, -0.459598, -0.465169, -0.470722, -0.476256,
    -0.481772, -0.487268, -0.492746, -0.498203, -0.503641, -0.509060, -0.514458, -0.519836, -0.525193, -0.530530,
    -0.535845, -0.541140, -0.546413, -0.551664, -0.556894, -0.562102, -0.567287, -0.572451, -0.577591, -0.582709,
    -0.587804, -0.592875, -0.597923, -0.602948, -0.607949, -0.612925, -0.617878, -0.622806, -0.627710, -0.632588,
    -0.637442, -0.642271, -0.647074, -0.651852, -0.656604, -0.661330, -0.666030, -0.670704, -0.675351, -0.679971,
    -0.684565, -0.689132, -0.693671, -0.698183, -0.702668, -0.707125, -0.711553, -0.715954, -0.720327, -0.724671,
    -0.728986, -0.733273, -0.737531, -0.741759, -0.745959, -0.750128, -0.754269, -0.758379, -0.762460, -0.766510,
    -0.770530, -0.774520, -0.778479, -0.782407, -0.786305, -0.790172, -0.794007, -0.797811, -0.801583, -0.805324,
    -0.809033, -0.812710, -0.816355, -0.819968, -0.823548, -0.827096, -0.830611, -0.834094, -0.837543, -0.840960,
    -0.844343, -0.847693, -0.851009, -0.854292, -0.857541, -0.860757, -0.863938, -0.867085, -0.870198, -0.873277,
    -0.876321, -0.879330, -0.882305, -0.885245, -0.888150, -0.891020, -0.893855, -0.896654, -0.899418, -0.902147,
    -0.904840, -0.907497, -0.910119, -0.912704, -0.915254, -0.917767, -0.920244, -0.922685, -0.925089, -0.927457,
    -0.929788, -0.932082, -0.934340, -0.936561, -0.938745, -0.940892, -0.943001, -0.945074, -0.947109, -0.949106,
    -0.951066, -0.952989, -0.954874, -0.956722, -0.958531, -0.960303, -0.962037, -0.963732, -0.965390, -0.967010,
    -0.968591, -0.970135, -0.971640, -0.973106, -0.974534, -0.975924, -0.977275, -0.978588, -0.979862, -0.981097,
    -0.982294, -0.983451, -0.984570, -0.985650, -0.986692, -0.987694, -0.988657, -0.989581, -0.990466, -0.991312,
    -0.992119, -0.992887, -0.993615, -0.994305, -0.994955, -0.995565, -0.996137, -0.996669, -0.997162, -0.997615,
    -0.998029, -0.998404, -0.998739, -0.999035, -0.999291, -0.999508, -0.999685, -0.999823, -0.999922, -0.999981,
    -1.000000, -0.999980, -0.999921, -0.999822, -0.999683, -0.999505, -0.999288, -0.999031, -0.998735, -0.998399,
    -0.998024, -0.997610, -0.997156, -0.996663, -0.996130, -0.995558, -0.994947, -0.994297, -0.993607, -0.992878,
    -0.992110, -0.991302, -0.990456, -0.989570, -0.988646, -0.987682, -0.986679, -0.985638, -0.984557, -0.983438,
    -0.982280, -0.981083, -0.979847, -0.978573, -0.977260, -0.975908, -0.974518, -0.973089, -0.971622, -0.970117,
    -0.968573, -0.966991, -0.965371, -0.963713, -0.962016, -0.960282, -0.958510, -0.956700, -0.954852, -0.952967,
    -0.951043, -0.949083, -0.947085, -0.945049, -0.942976, -0.940866, -0.938719, -0.936535, -0.934314, -0.932056,
    -0.929761, -0.927429, -0.925061, -0.922656, -0.920215, -0.917737, -0.915224, -0.912674, -0.910088, -0.907466,
    -0.904808, -0.902115, -0.899386, -0.896621, -0.893821, -0.890986, -0.888116, -0.885211, -0.882270, -0.879295,
    -0.876285, -0.873240, -0.870161, -0.867048, -0.863901, -0.860719, -0.857503, -0.854254, -0.850970, -0.847654,
    -0.844303, -0.840920, -0.837503, -0.834053, -0.830570, -0.827054, -0.823506, -0.819925, -0.816312, -0.812667,
    -0.808989, -0.805280, -0.801539, -0.797766, -0.793962, -0.790126, -0.786259, -0.782361, -0.778432, -0.774473,
    -0.770483, -0.766462, -0.762411, -0.758330, -0.754220, -0.750079, -0.745909, -0.741709, -0.737480, -0.733222,
    -0.728935, -0.724619, -0.720275, -0.715902, -0.711501, -0.707072, -0.702615, -0.698130, -0.693617, -0.689078,
    -0.684511, -0.679917, -0.675296, -0.670648, -0.665974, -0.661274, -0.656548, -0.651795, -0.647017, -0.642214,
    -0.637385, -0.632531, -0.627652, -0.622748, -0.617819, -0.612866, -0.607889, -0.602888, -0.597864, -0.592815,
    -0.587743, -0.582648, -0.577530, -0.572389, -0.567226, -0.562040, -0.556832, -0.551602, -0.546350, -0.541077,
    -0.535782, -0.530466, -0.525129, -0.519772, -0.514394, -0.508995, -0.503577, -0.498139, -0.492681, -0.487203,
    -0.481706, -0.476191, -0.470656, -0.465103, -0.459532, -0.453942, -0.448335, -0.442709, -0.437067, -0.431407,
    -0.425730, -0.420036, -0.414326, -0.408599, -0.402856, -0.397097, -0.391323, -0.385533, -0.379728, -0.373908,
    -0.368073, -0.362223, -0.356360, -0.350482, -0.344590, -0.338685, -0.332766, -0.326835, -0.320890, -0.314933,
    -0.308963, -0.302981, -0.296987, -0.290982, -0.284964, -0.278936, -0.272897, -0.266847, -0.260786, -0.254715,
    -0.248634, -0.242543, -0.236443, -0.230333, -0.224214, -0.218086, -0.211950, -0.205805, -0.199653, -0.193492,
    -0.187324, -0.181148, -0.174965, -0.168775, -0.162579, -0.156376, -0.150167, -0.143952, -0.137731, -0.131505,
    -0.125274, -0.119038, -0.112797, -0.106551, -0.100302, -0.094048, -0.087791, -0.081530, -0.075266, -0.069000,
    -0.062730, -0.056458, -0.050183, -0.043907, -0.037629, -0.031350, -0.025069, -0.018787, -0.012505, -0.006222, 0.000000
};


//線形補完処理
static float lerp(float x0, float y0, float x1, float y1, float x) {
  return -y0 + y0 + (y1 - y0) * (x - x0) / (x1 - x0) ;
}


//old version
// float calc_sin(uint32_t helz)
// {
//     uint32_t T = 1000 * 1000 / helz;
//     float R = 2 * PI * esp_timer_get_time() / T;
//
//     return sin(R);
// }


// new version
// サイン波形をテーブルから参照する
float calc_sin(uint32_t helz)
{
    float T = 1000000. / (float)helz;

    float TT = esp_timer_get_time() / T;
    float TT_shosu = TT - (int)TT;

    float R = PI2 * TT_shosu;

    float addr_f = R * 1000. / (PI2);
    int addr = (int)addr_f;
    float shosuR = addr_f - (int)addr;
    int addr1 = addr + 1;

    float hokan = lerp(0, sin_table[addr], 1, sin_table[addr1], shosuR);
    // printf("%f\n", hokan);
    return sin_table[addr] + hokan;
    // return sin_table[addr] + hokan;
}


//old version
// float calc_sin_float(float helz, float mod, uint64_t now_time_in_us)
// {
//     float T = 1000. * 1000. / helz;
//     float R = 2 * PI * now_time_in_us / T;
//
//     return sin(R + mod);
// }


// new version
// サイン波形をテーブルから参照する
float calc_sin_float(float helz, float mod, uint64_t now_time_in_us)
{
    float T = 1000000. / helz;      // 1000 * 1000 / helz

    float TT = now_time_in_us / T;
    float TT_shosu = TT - (int)TT;

    float R = PI2 * TT_shosu;       // 2 * PI * TT_shosu
    R = R + mod;
    //サイン対称性により + へ移行させる
    if(R < 0.0) R += PI2;

    float addr_f = R * 1000. / ((PI2)); //R * 1000. / ((2 * PI));
    int addr = (int)addr_f;
    if(addr >= 1000) {
        do {
            addr -= 1000;
        } while(addr >= 1000);
    }
    if(addr < 0) addr = 0;          //念の為 高速化のためなくてもいいかも

    float shosuR = addr_f - (int)addr;
    int addr1 = addr + 1;

    //引数 x0,x1は結局1の絶対値が必要なだけだから0,1とする
    float hokan = lerp(0, sin_table[addr], 1, sin_table[addr1], shosuR);

    return sin_table[addr] + hokan;
}


uint32_t herz_to_duty(float Y)
{
    uint32_t cal_pulsewidth = 0;
    cal_pulsewidth = (MAX_PULSEWIDTH - MIN_PULSEWIDTH) * Y;
    return cal_pulsewidth;
}
