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
    int8_t action;            // 动作类型：0-上车，1-下车，2-换乘, 3-经过
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


const Station* find_station_by_name(const char* station_name);

/**
 * @brief 查找两个站点之间的路径
 * @param start_name 起点站点名称
 * @param end_name 终点站点名称
 * @return 路径规划结果
 */
void find_route(const char* start_name, const char* end_name, Route* output_route);

#endif // ROUTE_PLANNER_H
