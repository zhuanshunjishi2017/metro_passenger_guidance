#include "metro_line.h"
#include "gui.h"
#include "timestruct.h"


const Station line_1[32] = {
//onlyid  id   x    y   trans   x'   y' draw  line name             pinyin
    {  0,   1,   5,   4,   0,   1,   0,   1,   1, "径河"          , "jinghe"         },
    {  1,   2,   5,   5,   0,   1,   0,   0,   1, "三店"          , "sandian"        },
    {  2,   3,   5,   6,   0,   1,   0,   0,   1, "码头潭公园"    , "matoutangongyuan"},
    {  3,   4,   5,   7,   0,   1,   0,   1,   1, "东吴大道"      , "dongwudadao"    },
    {  4,   5,   6,   8,   0,  -1,   1,   0,   1, "五环大道"      , "wuhuandadao"    },
    {  5,   6,   7,   9,   0,  -1,   1,   0,   1, "额头湾"        , "etouwan"        },
    {  6,   7,   8,  10,   0,  -1,   1,   0,   1, "竹叶海"        , "zhuyehai"       },
    {  7,   8,   9,  11,   0,  -1,   1,   0,   1, "舵落口"        , "duoluokou"      },
    {  8,   9,  10,  12,   0,  -1,   1,   0,   1, "古田一路"      , "gutianyilu"     },
    {  9,  10,  11,  13,   0,  -1,   1,   0,   1, "古田二路"      , "gutianerlu"     },
    { 10,  11,  12,  14,   0,  -1,   1,   0,   1, "古田三路"      , "gutiansanlu"    },
    { 11,  12,  13,  15,   0,  -1,   1,   0,   1, "古田四路"      , "gutiansilu"     },
    { 12,  13,  14,  16,   0,  -1,   1,   0,   1, "汉西一路"      , "hanxiyilu"      },
    { 13,  14,  15,  17,   3,  -1,   0,   1,   1, "宗关"          , "zongguan"       },
    { 14,  15,  16,  17,   0,   0,  -1,   0,   1, "太平洋"        , "taipingyang"    },
    { 15,  16,  17,  17,   0,   0,   1,   0,   1, "硚口路"        , "qiaokoulu"      },
    { 16,  17,  18,  17,   0,   0,  -1,   0,   1, "崇仁路"        , "chongrenlu"     },
    { 17,  18,  19,  17,   0,   0,   1,   1,   1, "利济北路"      , "lijibeilu"      },
    { 18,  19,  20,  16,   0,  -1,   0,   0,   1, "友谊路"        , "youyilu"        },
    { 19,  20,  21,  15,   2,  -1,   0,   0,   1, "循礼门"        , "xunlimen"       },
    { 20,  21,  22,  14,   0,   1,   1,   0,   1, "大智路"        , "dazhilu"        },
    { 21,  22,  23,  13,   0,   1,   1,   0,   1, "三阳路"        , "sanyanglu"      },
    { 22,  23,  24,  12,   0,   1,   1,   0,   1, "黄浦路"        , "huangpulu"      },
    { 23,  24,  25,  11,   0,   1,   1,   0,   1, "头道街"        , "toudaojie"      },
    { 24,  25,  26,  10,   0,   1,   1,   0,   1, "二七路"        , "erqilu"         },
    { 25,  26,  27,   9,   0,   1,   1,   0,   1, "徐州新村"      , "xuzhouxincun"   },
    { 26,  27,  28,   8,   0,   1,   1,   0,   1, "丹水池"        , "danshuichi"     },
    { 27,  28,  29,   7,   0,   1,   1,   0,   1, "新荣"          , "xinrong"        },
    { 28,  29,  30,   6,   0,   1,   1,   0,   1, "堤角"          , "dijiao"         },
    { 29,  30,  31,   5,   0,   1,   1,   1,   1, "滕子岗"        , "tengzigang"     },
    { 30,  31,  31,   4,   0,   1,   0,   0,   1, "滠口新城"      , "shekouxincheng" },
    { 31,  32,  31,   3,   0,   1,   0,   1,   1, "汉口北"        , "hankoubei"      }
};



const Station line_2[38] = {   
//onlyid  id   x    y   trans   x'   y' draw  line name             pinyin
    { 32,   1,  18,   1,   0,   1,   0,   1,   2, "天河机场"        , "tianhejichang"    },
    { 33,   2,  18,   2,   0,   1,  -1,   1,   2, "航空总部"        , "hangkongzongbu"   },
    { 34,   3,  19,   3,   0,   1,  -1,   0,   2, "宋家岗"          , "songjiagang"      },
    { 35,   4,  20,   4,   0,   1,  -1,   0,   2, "巨龙大道"        , "julongdadao"      },
    { 36,   5,  21,   5,   0,   1,  -1,   1,   2, "盘龙城"          , "panlongcheng"     },
    { 37,   6,  21,   6,   3,   0,   1,   1,   2, "宏图大道"        , "hongtudadao"      },
    { 38,   7,  19,   6,   0,  -1,  -1,   1,   2, "常青城"          , "changqingcheng"   },
    { 39,   8,  18,   7,   0,  -1,   0,   1,   2, "金银潭"          , "jinyintan"        },
    { 40,   9,  18,   8,   0,  -1,   0,   0,   2, "常青花园"        , "changqinghuayuan" },
    { 41,  10,  18,   9,   0,  -1,   0,   0,   2, "长港路"          , "changganglu"      },
    { 42,  11,  18,  10,   0,  -1,   0,   0,   2, "汉口火车站"      , "hankouhuochezhan" },
    { 43,  12,  18,  12,   3,  -1,  -1,   0,   2, "范湖"            , "fanhu"            },
    { 44,  13,  18,  13,   0,   1,   0,   1,   2, "王家墩东"        , "wangjiadundong"   },
    { 45,  14,  19,  14,   0,   0,   1,   1,   2, "青年路"          , "qingnianlu"       },
    { 46,  15,  20,  14,   0,   0,  -1,   1,   2, "中山公园"        , "zhongshangongyuan"},
    { 19,  16,  21,  15,  -1,  -1,   0,   0,   2, "循礼门"          , "xunlimen"         },
    { 48,  17,  22,  16,   0,   1,  -1,   0,   2, "江汉路"          , "jianghanlu"       },
    { 49,  18,  24,  18,   0,   1,  -1,   0,   2, "积玉桥"          , "jiyuqiao"         },
    { 50,  19,  25,  19,   0,   1,  -1,   0,   2, "螃蟹岬"          , "pangxiejia"       },
    { 51,  20,  26,  20,   0,   1,  -1,   0,   2, "小龟山"          , "xiaoguishan"      },
    { 52,  21,  27,  21,   4,   1,   1,   1,   2, "洪山广场"        , "hongshanguangchang"},
    { 53,  22,  26,  22,   4,  -1,   1,   1,   2, "中南路"          , "zhongnanlu"       },
    { 54,  23,  27,  23,   0,  -1,   1,   0,   2, "宝通寺"          , "baotongsi"        },
    { 55,  24,  28,  24,   0,  -1,   1,   0,   2, "街道口"          , "jiedaokou"        },
    { 56,  25,  29,  25,   0,  -1,   1,   0,   2, "广埠屯"          , "guangbutun"       },
    { 57,  26,  30,  26,   0,  -1,   1,   0,   2, "虎泉"            , "huquan"           },
    { 58,  27,  31,  27,   0,  -1,   1,   1,   2, "杨家湾"          , "yangjiawan"       },
    { 59,  28,  32,  27,   0,   0,   1,   0,   2, "光谷广场"        , "guangguguangchang"},
    { 60,  29,  33,  27,   0,   0,  -1,   0,   2, "珞雄路"          , "luoxionglu"       },
    { 61,  30,  34,  27,   0,   0,   1,   0,   2, "华中科技大学"    , "huazhongkejidaxue"},
    { 62,  31,  35,  27,   0,   0,  -1,   1,   2, "光谷大道"        , "guanggudadao"     },
    { 63,  32,  36,  28,   0,   1,   0,   1,   2, "佳园路"          , "jiayuanlu"        },
    { 64,  33,  36,  29,   0,   1,   0,   0,   2, "武汉东站"        , "wuhandongzhan"    },
    { 65,  34,  36,  30,   0,   1,   0,   0,   2, "黄龙山路"        , "huanglongshanlu"  },
    { 66,  35,  36,  31,   0,   1,   0,   0,   2, "金融港北"        , "jinronggangbei"   },
    { 67,  36,  36,  32,   0,   1,   0,   1,   2, "秀湖"            , "xiuhu"            },
    { 68,  37,  37,  33,   0,   0,   1,   1,   2, "藏龙东街"        , "zanglongdongjie"  },
    { 69,  38,  38,  33,   0,   1,   0,   1,   2, "佛祖岭"          , "fozuling"         }
};



const Station line_3[24] = {
//onlyid  id   x    y   trans   x'   y' draw  line name             pinyin
    { 70,   1,   5,  27,   0,   1,   1,   1,   3, "沌阳大道"      , "zhuanyangdadao"  },
    { 71,   2,   6,  26,   0,   1,   1,   0,   3, "东风公司"      , "dongfenggongsi"  },
    { 72,   3,   7,  25,   0,   1,   1,   0,   3, "体育中心"      , "tiyuzhongxin"    },
    { 73,   4,   8,  24,   0,   1,   1,   0,   3, "三角湖"        , "sanjiaohu"       },
    { 74,   5,   9,  23,   0,   1,   1,   0,   3, "汉阳客运站"    , "hanyangkeyunzhan"},
    { 75,   6,  10,  22,   0,   1,   1,   0,   3, "四新大道"      , "sixindadao"      },
    { 76,   7,  11,  21,   0,   1,   1,   0,   3, "陶家岭"        , "taojialing"      },
    { 77,   8,  12,  20,   0,   1,   1,   0,   3, "龙阳村"        , "longyangcun"     },
    { 78,   9,  13,  19,   4,   1,   0,   0,   3, "王家湾"        , "wangjiawan"      },
    { 13,  10,  15,  17,  -1,  -1,   0,   1,   3, "宗关"          , "zongguan"        },
    { 80,  11,  15,  14,   0,  -1,  -1,   1,   3, "双墩"          , "shuangdun"       },
    { 81,  12,  16,  13,   0,  -1,  -1,   0,   3, "武汉商务区"    , "wuhanshangwuqu"  },
    { 82,  13,  17,  12,   0,  -1,  -1,   1,   3, "云飞路"        , "yunfeilu"        },
    { 43,  14,  18,  12,  -2,  -1,  -1,   0,   3, "范湖"          , "fanhu"           },
    { 83,  15,  19,  12,   0,   0,  -1,   0,   3, "菱角湖"        , "lingjiaohu"      },
    { 84,  16,  20,  12,   0,   1,   1,   1,   3, "香港路"        , "xiangganglu"     },
    { 85,  17,  21,  11,   0,   1,   1,   0,   3, "惠济二路"      , "huijierlu"       },
    { 86,  18,  22,  10,   0,   1,   1,   0,   3, "赵家条"        , "zhaojiatiao"     },
    { 87,  19,  23,   9,   0,   1,   1,   0,   3, "罗家庄"        , "luojiazhuang"    },
    { 88,  20,  24,   8,   0,   1,   1,   1,   3, "二七小路"      , "erqixiaolu"      },
    { 89,  21,  24,   7,   0,   1,   0,   0,   3, "兴业路"        , "xingyelu"        },
    { 90,  22,  24,   6,   0,   1,   0,   1,   3, "后湖大道"      , "houhudadao"      },
    { 91,  23,  23,   6,   0,   0,  -1,   0,   3, "市民之家"      , "shiminzijia"     },
    { 37,  24,  21,   6,  -2,   0,   1,   1,   3, "宏图大道"      , "hongtudadao"     }
};



const Station line_4[37] = {
//onlyid  id   x    y   trans   x'   y' draw  line name             pinyin
    { 92,   1,   1,  17,   0,  -1,   0,   1,   4, "柏林"          , "bailin"             },
    { 93,   2,   2,  17,   0,   0,  -1,   0,   4, "新庙村"        , "xinmiaocun"         },
    { 94,   3,   3,  17,   0,   0,   1,   0,   4, "临嶂大道"      , "linzhangdadao"      },
    { 95,   4,   4,  17,   0,   0,  -1,   1,   4, "蔡甸广场"      , "caidianguangchang"  },
    { 96,   5,   5,  18,   0,  -1,   1,   0,   4, "凤凰路"        , "fenghuanglu"        },
    { 97,   6,   6,  19,   0,  -1,   1,   0,   4, "新农"          , "xinnong"            },
    { 98,   7,   7,  20,   0,  -1,   1,   1,   4, "知音"          , "zhiyin"             },
    { 99,   8,   8,  20,   0,   0,  -1,   0,   4, "集贤"          , "jixian"             },
    {100,   9,   9,  20,   0,   1,   1,   1,   4, "新天"          , "xintian"            },
    {101,  10,   9,  19,   0,  -1,  -1,   1,   4, "黄金口"        , "huangjinkou"        },
    {102,  11,  10,  19,   0,   0,  -1,   0,   4, "孟家铺"        , "mengjiapub"         },
    {103,  12,  11,  19,   0,   0,   1,   0,   4, "永安堂"        , "yongantang"         },
    {104,  13,  12,  19,   0,   0,  -1,   0,   4, "玉龙路"        , "yulonglu"           },
    { 78,  14,  13,  19,  -3,   1,   0,   1,   4, "王家湾"        , "wangjiawan"         },
    {105,  15,  14,  20,   0,   0,   1,   1,   4, "十里铺"        , "shilipu"            },
    {106,  16,  15,  20,   0,   0,  -1,   0,   4, "七里庙"        , "qilimiao"           },
    {107,  17,  16,  20,   0,   0,   1,   0,   4, "五里墩"        , "wulidun"            },
    {108,  18,  17,  20,   0,   0,  -1,   0,   4, "汉阳火车站"    , "hanyanghuochezhan"  },
    {109,  19,  18,  20,   0,   0,   1,   0,   4, "钟家村"        , "zhongjiacun"        },
    {110,  20,  19,  20,   0,   0,  -1,   1,   4, "拦江路"        , "lanjianglu"         },
    {111,  21,  21,  22,   0,   0,   1,   1,   4, "复兴路"        , "fuxinglu"           },
    {112,  22,  22,  22,   0,   0,  -1,   0,   4, "首义路"        , "shouyilu"           },
    {113,  23,  23,  22,   0,   0,   1,   0,   4, "武昌火车站"    , "wuchanghuochezhan"  },
    {114,  24,  24,  22,   0,   0,  -1,   0,   4, "梅苑小区"      , "meiyuanxiaoqu"      },
    { 53,  25,  26,  22,  -2,  -1,   1,   1,   4, "中南路"        , "zhongnanlu"         },
    { 52,  26,  27,  21,  -2,   1,   1,   1,   4, "洪山广场"      , "hongshanguangchang" },
    {115,  27,  28,  20,   0,   1,   1,   0,   4, "楚河汉街"      , "chuhehanjie"        },
    {116,  28,  29,  19,   0,   1,   1,   0,   4, "青鱼嘴"        , "qingyuzui"          },
    {117,  29,  30,  18,   0,   1,   1,   0,   4, "东亭"          , "dongting"           },
    {118,  30,  31,  17,   0,   1,   1,   0,   4, "岳家嘴"        , "yuejiazui"          },
    {119,  31,  32,  16,   0,   1,   1,   0,   4, "铁机路"        , "tiejilu"            },
    {120,  32,  33,  15,   0,   1,   1,   0,   4, "罗家港"        , "luojiagang"         },
    {121,  33,  34,  14,   0,   1,   1,   0,   4, "园林路"        , "yuanlinlu"          },
    {122,  34,  35,  13,   0,   1,   1,   0,   4, "仁和路"        , "renhelu"            },
    {123,  35,  36,  12,   0,   1,   1,   0,   4, "工业四路"      , "gongyesilu"         },
    {124,  36,  37,  11,   0,   1,   1,   0,   4, "杨春湖"        , "yangchunhu"         },
    {125,  37,  38,  10,   0,   1,   1,   1,   4, "武汉火车站"    , "wuhanhuochezhan"    }
};


//下面定义每一站之间的时间间隔
const uint16_t time_line_1[31] = {
    99, 176, 93, 148, 148, 84, 72, 128, 136, 81, 71, 
    73, 82, 141, 93, 102, 79, 79, 88, 97, 90, 103, 
    107, 82, 71, 136, 128, 104, 92, 100, 304};

Timetable timetable_line_1 = {{6, 0 ,0}, {23, 0, 0}, 313};

const uint16_t time_line_2[37] = {
    600, 143, 204, 164, 395, 289, 201, 109, 
    173, 141, 122, 141, 100, 95, 154, 90, 
    329, 158, 93, 117, 97, 142, 124, 95, 161, 
    144, 132, 79, 101, 82, 137, 149, 106, 204, 
    160, 200, 102};

Timetable timetable_line_2 = {{6, 0 ,0}, {23, 0, 0}, 248};


const uint16_t time_line_3[23] = {
    145, 84, 145, 156, 93, 136, 91,
    111, 253, 98, 183, 123, 89, 168, 
    93, 93, 90, 88, 76, 125, 118, 167, 242};

Timetable timetable_line_3 = {{6, 0 ,0}, {23, 0, 0}, 372};


const uint16_t time_line_4[36] = {
    94, 155, 103, 91, 98, 86, 94, 93, 129, 100, 
    75, 85, 79, 84, 60, 56, 70, 71, 68, 82, 58, 
    57, 57, 50, 50, 59, 52, 58, 56, 73, 86, 88,
    69, 91, 107, 78};

Timetable timetable_line_4 = {{6, 0 ,0}, {23, 0, 0}, 255};



const MetroLine metro_lines[LINE_COUNT] = {
    {line_1, 32, COLOR_LINE1, 1, time_line_1, &timetable_line_1},
    {line_2, 38, COLOR_LINE2, 2, time_line_2, &timetable_line_2},
    {line_3, 24, COLOR_LINE3, 3, time_line_3, &timetable_line_3},
    {line_4, 37, COLOR_LINE4, 4, time_line_4, &timetable_line_4},
};



const MetroLine * get_metro_line(int8_t line_number)
{
    for (int8_t i = 0; i < LINE_COUNT; i++)
    {
        if (metro_lines[i].line_number == line_number)
        {
            return metro_lines + i;
        }
    }
    return NULL;
}

const Station * get_station(const Station * sta)
{
    for (int8_t i = 0; i < LINE_COUNT; i++)
    {
        for (int j = 0; j < metro_lines[i].count; j++)
        {
            if( metro_lines[i].stations[j].only_id == sta->only_id
                && metro_lines[i].stations[j].line_belonged == sta->line_belonged)
                return &metro_lines[i].stations[j];
        }
    }
    return NULL;

}

const Station * get_transfer_station(const Station * src, const Station ** first_dist)
{
    if(!src->is_transfer)
    {
        *first_dist = NULL;
        return NULL;
    }

    int8_t is_find_first = 0;
    for (int8_t i = 0; i < LINE_COUNT; i++)
    {
        for (int j = 0; j < metro_lines[i].count; j++)
        {
            if( !is_find_first 
                && metro_lines[i].stations[j].only_id == src->only_id
                && metro_lines[i].stations[j].line_belonged != src->line_belonged)
            {
                *first_dist = &metro_lines[i].stations[j];
                is_find_first = 1;
            }
            else if (is_find_first
                && metro_lines[i].stations[j].only_id == src->only_id
                && metro_lines[i].stations[j].line_belonged != src->line_belonged
                && metro_lines[i].stations[j].line_belonged != (*first_dist)->line_belonged)
            {
                return &metro_lines[i].stations[j];
            }
        }
    }
    return NULL;

}


