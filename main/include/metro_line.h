#ifndef METRO_LINE
#define METRO_LINE

#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include <stdint.h>

#define SCREEN_W 1024
#define SCREEN_H 600  // 屏幕分辨率

#define GEO_X_MIN 0
#define GEO_X_MAX 45
#define GEO_Y_MIN 0
#define GEO_Y_MAX 35  // GeoGebra 坐标范围
#define PAD_X 12
#define PAD_Y 40  // 屏幕边距
#define STATION_RADIUS 5 //半径
#define TRANSFER_STATION_RADIUS 7
#define LINE_WIDTH 5  
#define OUTLINE_WIDTH 2
#define LABEL_OFFSET 4
#define TEXT_MAX 100


#define COLOR_LINE1    0x0066A4  // 1号线 蓝色
#define COLOR_LINE2    0xE99CC0  // 2号线 粉色
#define COLOR_LINE3    0xD7B665  // 3号线 橙色
#define COLOR_LINE4    0xBAD200  // 4号线 绿色

typedef struct {
    const int8_t id;
    const char* name;
    const char* name_pinyin;
    lv_coord_t geo_x;
    lv_coord_t geo_y;
    int8_t is_transfer;        //是否为换乘站
    int8_t horizon_offset;   //标签水平偏移量，右为正
    int8_t vertical_offset;  //标签垂直偏移量，下为正
    int8_t is_draw_point;      //是否为端点
} Station;                   // 站点结构体 两个偏移皆为0则不绘制（换乘车站）


typedef struct {
    const Station* stations;
    uint16_t count;
    uint32_t line_color;
    uint8_t draw_point_count;
} MetroLine; // 线路结构体

const Station line_1[32] = {
    {1, "径河", "jinghe",                 5, 4,   0, 1, 0, 1},
    {2, "三店", "sandian",                5, 5,   0, 1, 0, 0},
    {3, "码头潭公园", "matoutangongyuan", 5, 6,   0, 1, 0, 0},
    {4, "东吴大道", "dongwudadao",        5, 7,   0, 1, 0, 1},
    {5, "五环大道", "wuhuandadao",        6, 8,   0, -1, 1, 0},
    {6, "额头湾", "etouwan",              7, 9,   0, -1, 1, 0},
    {7, "竹叶海", "zhuyehai",             8, 10,  0, -1, 1, 0},
    {8, "舵落口", "duoluokou",            9, 11,  0, -1, 1, 0},
    {9, "古田一路", "gutianyilu",         10, 12, 0, -1, 1, 0},
    {10, "古田二路", "gutianerlu",        11, 13, 0, -1, 1, 0},
    {11, "古田三路", "gutiansanlu",       12, 14, 0, -1, 1, 0},
    {12, "古田四路", "gutiansilu",        13, 15, 0, -1, 1, 0},
    {13, "汉西一路", "hanxiyilu",         14, 16, 1, -1, 0, 0},
    {14, "宗关", "zongguan",              15, 17, 0, 0, 1, 1},
    {15, "太平洋", "taipingyang",         16, 17, 0, 0, -1, 0},
    {16, "硚口路", "qiaokoulu",           17, 17, 0, 0, 1, 0},
    {17, "崇仁路", "chongrenlu",          18, 17, 0, 0, -1, 0},
    {18, "利济北路", "lijibeilu",         19, 17, 0, 0, 1, 1},
    {19, "友谊路", "youyilu",             20, 16, 0, -1, 0, 0},
    {20, "循礼门", "xunlimen",            21, 15, 1, -1, 0, 0},
    {21, "大智路", "dazhilu",             22, 14, 0, 1, 1, 0},
    {22, "三阳路", "sanyanglu",           23, 13, 0, 1, 1, 0},
    {23, "黄浦路", "huangpulu",           24, 12, 0, 1, 1, 0},
    {24, "头道街", "toudaojie",           25, 11, 0, 1, 1, 0},
    {25, "二七路", "erqilu",              26, 10, 0, 1, 1, 0},
    {26, "徐州新村", "xuzhouxincun",      27, 9, 0, 1, 1, 0},
    {27, "丹水池", "danshuichi",          28, 8, 0, 1, 1, 0},
    {28, "新荣", "xinrong",               29, 7, 0, 1, 1, 0},
    {29, "堤角", "dijiao",                30, 6, 0, 1, 1, 0},
    {30, "滕子岗", "tengzigang",          31, 5, 0, 1, 1, 1},
    {31, "滠口新城", "shekouxincheng",    31, 4, 0, 1, 0, 0},
    {32, "汉口北", "hankoubei",           31, 3, 0, 1, 0, 1}
};
const Station line_2[38] = {   
    {1, "天河机场", "tianhejichang",          17, 0, 0, 1, 0, 1},
    {2, "航空总部", "hangkongzongbu",         17, 2, 0, 1,-1, 1},
    {3, "宋家岗", "songjiagang",              18, 3, 0, 1,-1, 0},
    {4, "巨龙大道", "julongdadao",            19, 4, 0, 1,-1, 0},
    {5, "盘龙城", "panlongcheng",             20, 5, 0, 1,-1, 1},
    {6, "宏图大道", "hongtudadao",            20, 6, 1, 0, 1, 1},
    {7, "常青城", "changqingcheng",           19, 6, 0,-1,-1, 1},
    {8, "金银潭", "jinyintan",                18, 7, 0,-1, 0, 1},
    {9, "常青花园", "changqinghuayuan",       18, 8, 0,-1, 0, 0},
    {10, "长港路", "changganglu",             18, 9, 0,-1, 0, 0},
    {11, "汉口火车站", "hankouhuochezhan",    18, 10, 0,-1, 0, 0},
    {12, "范湖", "fanhu",                     18, 12, 1,-1,-1, 0},
    {13, "王家墩东", "wangjiadundong",        18, 13, 0, 1, 0, 1},
    {14, "青年路", "qingnianlu",              19, 14, 0, 0, 1, 1},
    {15, "中山公园", "zhongshangongyuan",     20, 14, 0, 0,-1, 1},
    {16, "循礼门", "xunlimen",                21, 15, 1, 0, 0, 0},
    {17, "江汉路", "jianghanlu",              22, 16, 0, 1,-1, 0},
    {18, "积玉桥", "jiyuqiao",                24, 18, 0, 1,-1, 0},
    {19, "螃蟹岬", "pangxiejia",              25, 19, 0, 1,-1, 0},
    {20, "小龟山", "xiaoguishan",             26, 20, 0, 1,-1, 0},
    {21, "洪山广场", "hongshanguangchang",    27, 21, 1, 1, 1, 1},
    {22, "中南路", "zhongnanlu",              26, 22, 1,-1, 1, 1},
    {23, "宝通寺", "baotongsi",               27, 23, 0,-1, 1, 0},
    {24, "街道口", "jiedaokou",               28, 24, 0,-1, 1, 0},
    {25, "广埠屯", "guangbutun",              29, 25, 0,-1, 1, 0},
    {26, "虎泉", "huquan",                    30, 26, 0,-1, 1, 0},
    {27, "杨家湾", "yangjiawan",              31, 27, 0,-1, 1, 1},
    {28, "光谷广场", "guangguguangchang",     32, 27, 0, 0, 1, 0},
    {29, "珞雄路", "luoxionglu",              33, 27, 0, 0,-1, 0},
    {30, "华中科技大学", "huazhongkejidaxue", 34, 27, 0, 0, 1, 0},
    {31, "光谷大道", "guanggudadao",          35, 27, 0, 0,-1, 1},
    {32, "佳园路", "jiayuanlu",               36, 28, 0, 1, 0, 1},
    {33, "武汉东站", "wuhandongzhan",         36, 29, 0, 1, 0, 0},
    {34, "黄龙山路", "huanglongshanlu",       36, 30, 0, 1, 0, 0},
    {35, "金融港北", "jinronggangbei",        36, 31, 0, 1, 0, 0},
    {36, "秀湖", "xiuhu",                     36, 32, 0, 1, 0, 1},
    {37, "藏龙东街", "zanglongdongjie",       37, 33, 0, 0, 1, 1},
    {38, "佛祖岭", "fozuling",                38, 33, 0, 1, 0, 1}
};
const Station line_3[24] = {
    {1, "沌阳大道", "zhuanyangdadao",         4, 26, 0, 1, 1, 1},
    {2, "东风公司", "dongfenggongsi",         5, 25, 0, 1, 1, 0},
    {3, "体育中心", "tiyuzhongxin",           6, 24, 0, 1, 1, 0},
    {4, "三角湖", "sanjiaohu",                7, 23, 0, 1, 1, 0},
    {5, "汉阳客运站", "hanyangkeyunzhan",     8, 22, 0, 1, 1, 0},
    {6, "四新大道", "sixindadao",             9, 21, 0, 1, 1, 0},
    {7, "陶家岭", "taojialing",              10, 20, 0, 1, 1, 0},
    {8, "龙阳村", "longyangcun",             11, 19, 0, 1, 1, 0},
    {9, "王家湾", "wangjiawan",              12, 18, 1, 1, 0, 0},
    {10, "宗关", "zongguan",                 14, 16, 1, 0, 0, 0},
    {11, "双墩", "shuangdun",                15, 15, 0,-1,-1, 0},
    {12, "武汉商务区", "wuhanshangwuqu",     16, 14, 0,-1,-1, 0},
    {13, "云飞路", "yunfeilu",               17, 13, 0,-1,-1, 0},
    {14, "范湖", "fanhu",                    18, 12, 1, 0, 0, 1},
    {15, "菱角湖", "lingjiaohu",             19, 12, 0, 0,-1, 0},
    {16, "香港路", "xiangganglu",            20, 12, 0, 1, 1, 1},
    {17, "惠济二路", "huijierlu",            21, 11, 0, 1, 1, 0},
    {18, "赵家条", "zhaojiatiao",            22, 10, 0, 1, 1, 0},
    {19, "罗家庄", "luojiazhuang",           23, 9,  0, 1, 1, 0},
    {20, "二七小路", "erqixiaolu",           24, 8,  0, 1, 1, 1},
    {21, "兴业路", "xingyelu",               24, 7,  0, 1, 0, 0},
    {22, "后湖大道", "houhudadao",           24, 6,  0, 1, 0, 1},
    {23, "市民之家", "shiminzijia",          22, 6,  0, 0,-1, 0},
    {24, "宏图大道", "hongtudadao",          20, 6,  1, 0, 0, 1}
};
const Station line_4[37] = {
    {1, "柏林", "bailin",                    0, 16, 0,-1, 0, 1},
    {2, "新庙村", "xinmiaocun",              1, 16, 0, 0,-1, 0},
    {3, "临嶂大道", "linzhangdadao",         2, 16, 0, 0, 1, 0},
    {4, "蔡甸广场", "caidianguangchang",     3, 16, 0, 0,-1, 1},
    {5, "凤凰路", "fenghuanglu",             4, 17, 0,-1, 1, 0},
    {6, "新农", "xinnong",                   5, 18, 0,-1, 1, 0},
    {7, "知音", "zhiyin",                    6, 19, 0,-1, 1, 1},
    {8, "集贤", "jixian",                    7, 19, 0, 0,-1, 0},
    {9, "新天", "xintian",                   8, 19, 0, 1, 1, 1},
    {10, "黄金口", "huangjinkou",            8, 18, 0,-1, -1, 1},
    {11, "孟家铺", "mengjiapub",             9, 18, 0, 0,-1, 0},
    {12, "永安堂", "yongantang",            10, 18, 0, 0, 1, 0},
    {13, "玉龙路", "yulonglu",              11, 18, 0, 0,-1, 0},
    {14, "王家湾", "wangjiawan",            12, 18, 1, 0, 0, 1},
    {15, "十里铺", "shilipu",               13, 19, 0, 0, 1, 1},
    {16, "七里庙", "qilimiao",              14, 19, 0, 0,-1, 0},
    {17, "五里墩", "wulidun",               15, 19, 0, 0, 1, 0},
    {18, "汉阳火车站", "hanyanghuochezhan", 16, 19, 0, 0,-1, 0},
    {19, "钟家村", "zhongjiacun",           17, 19, 0, 0, 1, 0},
    {20, "拦江路", "lanjianglu",            18, 19, 0, 0,-1, 1},
    {21, "复兴路", "fuxinglu",              21, 22, 0, 0, 1, 1},
    {22, "首义路", "shouyilu",              22, 22, 0, 0,-1, 0},
    {23, "武昌火车站", "wuchanghuochezhan", 23, 22, 0, 0, 1, 0},
    {24, "梅苑小区", "meiyuanxiaoqu",       24, 22, 0, 0,-1, 0},
    {25, "中南路", "zhongnanlu",            26, 22, 1, 0, 0, 1},
    {26, "洪山广场", "hongshanguangchang",  27, 21, 1, 0, 0, 0},
    {27, "楚河汉街", "chuhehanjie",         28, 20, 0, 1, 1, 0},
    {28, "青鱼嘴", "qingyuzui",             29, 19, 0, 1, 1, 0},
    {29, "东亭", "dongting",                30, 18, 0, 1, 1, 0},
    {30, "岳家嘴", "yuejiazui",             31, 17, 0, 1, 1, 0},
    {31, "铁机路", "tiejilu",               32, 16, 0, 1, 1, 0},
    {32, "罗家港", "luojiagang",            33, 15, 0, 1, 1, 0},
    {33, "园林路", "yuanlinlu",             34, 14, 0, 1, 1, 0},
    {34, "仁和路", "renhelu",               35, 13, 0, 1, 1, 0},
    {35, "工业四路", "gongyesilu",          36, 12, 0, 1, 1, 0},
    {36, "杨春湖", "yangchunhu",            37, 11, 0, 1, 1, 0},
    {37, "武汉火车站", "wuhanhuochezhan",   38, 10, 0, 1, 1, 1}
};

const MetroLine metro_lines[4] = {
    {line_1, 32, COLOR_LINE1, 6},
    {line_2, 38, COLOR_LINE2, 17},
    {line_3, 24, COLOR_LINE3, 6},
    {line_4, 37, COLOR_LINE4, 11},
};

uint16_t magnify_size = 30;

lv_coord_t origin_x = -30;
lv_coord_t origin_y = -30;

extern const lv_font_t heiti_14;

static lv_coord_t geo_to_screen(lv_coord_t pos, lv_coord_t origin);
static void draw_metro_line(lv_obj_t* canvas, const MetroLine* line);
static void draw_station(lv_obj_t* canvas, const Station* s, lv_color_t color);
void create_metro_map(void);


#endif
