#ifndef ROUTE_PLANNER_H
#define ROUTE_PLANNER_H

#include "metro_line.h"
#include <stdint.h>
#include <stdbool.h>

#define MAX_STATIONS 150       // 最大站点数
#define MAX_PATH_LENGTH 50     // 最大路径长度
#define MAX_TRANSFER_COUNT 10  // 最大换乘次数

// 路径步骤结构体
typedef struct {
    char* station_name;       // 站点名称
    int8_t line_number;       // 当前所在线路
    int8_t action;            // 动作类型：0-上车，1-下车，2-换乘
    bool is_transfer;         // 是否为换乘站
} RouteStep;

// 路径结构体
typedef struct {
    RouteStep steps[MAX_PATH_LENGTH];  // 路径步骤数组
    int step_count;                    // 步骤数量
    int total_stations;                // 总站点数
    int transfer_count;                // 换乘次数
    int distance;                      // 距离（站数）
} Route;

// 路径规划结果
typedef struct {
    Route routes[3];          // 最多返回3条路径
    int route_count;          // 找到的路径数量
} RouteResult;

const Station* find_station_by_name(const char* station_name);
RouteResult find_route(const char* start_name, const char* end_name);
void get_route_description(const Route* route, char* buffer, int buffer_size);
/**
 * @brief 初始化路径规划器
 */
void route_planner_init(void);

/**
 * @brief 查找两个站点之间的路径
 * @param start_name 起点站点名称
 * @param end_name 终点站点名称
 * @return 路径规划结果
 */
RouteResult find_route(const char* start_name, const char* end_name);

/**
 * @brief 查找两个站点之间的最佳路径（最少换乘）
 * @param start_name 起点站点名称
 * @param end_name 终点站点名称
 * @return 最佳路径
 */
Route find_best_route(const char* start_name, const char* end_name);

/**
 * @brief 根据站点指针查找路径
 * @param start_station 起点站点指针
 * @param end_station 终点站点指针
 * @return 路径规划结果
 */
RouteResult find_route_by_station(const Station* start_station, const Station* end_station);

/**
 * @brief 清空路径规划结果
 * @param result 要清空的路径结果
 */
void clear_route_result(RouteResult* result);

/**
 * @brief 打印路径信息（用于调试）
 * @param route 要打印的路径
 */
void print_route(const Route* route);

/**
 * @brief 获取路径的描述文本
 * @param route 路径
 * @param buffer 输出缓冲区
 * @param buffer_size 缓冲区大小
 */
void get_route_description(const Route* route, char* buffer, int buffer_size);

#endif // ROUTE_PLANNER_H
