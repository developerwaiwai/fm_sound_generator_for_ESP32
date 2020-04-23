

enum command {
    ALGORISM_TRANS  = 0x01,
    NOTE_ON         = 0x02,
    NOTE_OFF        = 0x03,

    PLAY_START      = 0xFE,
    PLAY_STOP       = 0xFF
};

enum note_type {
    DOTTED_WHOLE  = 3000000,
    WHOLE         = 2000000,
    TOTTED_HALF   = 1500000,
    HALF          = 1000000,
    DOTTED_QUOTER =  750000,
    QUOTER        =  500000,
    DOTTED_EIGHTH =  375000,
    EIGHTH        =  250000,
    SIXTEENTH     =  125000,
    TRIPLE_EIGHTH =  166666,
    TRIPLE_16TH   =   83333
};

struct tone_color_data {
    uint16_t    op_num;         // オペレータ番号
    uint16_t    amp;            // 出力強度 実際はこの値を1/10した値(float)
    uint32_t    helz_nul;       // 基本周波数の倍数
    uint64_t    attack_time_us; // アタック時間(us)
    uint64_t    decay_time_us;  // ディケイ時間(us)
    uint16_t    sustain_level;  // サスティーンレベル 実際はこの値を1/10した値(float)
    uint16_t    releasetime_us; // リリース時間　現在は使わない

    uint8_t     algorism_number;//使用アルゴリズム番号(0-7)
    uint8_t     padding2[3];    //パディング
};

struct note_on_off_data {
    bool        on_off;         // ture = on, false = off
    uint8_t     padding[3];
    uint16_t    tempo;          // テンポ
    uint16_t    padding1;       // パディング
    uint8_t     note_type;      // 音符の種類
    uint8_t     padding2[3];    // パディング
    uint32_t    padding3[5];    // パディング
};

struct packet {
    unsigned char command;
    unsigned char data[32];
    unsigned char padding[3];
};
