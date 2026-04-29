#include "route_planner.h"
#include <string.h>
#include <stdlib.h>

extern const MetroLine metro_lines[4];

// 简化的图节点结构
typedef struct {
    int station_id;
    int line_id;
    int distance;
    int transfers;
    int parent_index;
} PathNode;

// 全局变量
static PathNode path_nodes[MAX_STATIONS * 4]; // 存储所有搜索过的节点
static int path_node_count = 0; // 当前节点数量
static int path_parent[MAX_STATIONS * 4]; // 用于回溯路径
static int path_line[MAX_STATIONS * 4];   // 线路信息
static int path_distance[MAX_STATIONS * 4]; // 距离信息

/**
 * @brief 清空路径搜索状态
 */
static void clear_search_state(void)
{
    path_node_count = 0;
    memset(path_parent, -1, sizeof(path_parent));
    memset(path_line, -1, sizeof(path_line));
    memset(path_distance, -1, sizeof(path_distance));
}

/**
 * @brief 检查站点是否在指定线路上
 * @param station_id 站点ID
 * @param line_number 线路号
 * @return 是否在该线路上
 */
static bool is_station_on_line(int station_id, int line_number)
{
    if (line_number < 1 || line_number > 4) return false;

    for (int i = 0; i < metro_lines[line_number - 1].count; i++) {
        if (metro_lines[line_number - 1].stations[i].id == station_id) {
            return true;
        }
    }
    return false;
}

/**
 * @brief 根据站点名称查找站点指针
 * @param station_name 站点名称
 * @return 站点指针，如果不存在返回NULL
 */
const Station* find_station_by_name(const char* station_name)
{
    if (!station_name || strlen(station_name) == 0) {
        return NULL;
    }

    for (int line = 0; line < 4; line++) {
        for (int i = 0; i < metro_lines[line].count; i++) {
            if (strcmp(metro_lines[line].stations[i].name, station_name) == 0) {
                return &metro_lines[line].stations[i];
            }
        }
    }
    return NULL;
}

/**
 * @brief 根据站点ID查找站点指针
 * @param station_id 站点ID
 * @return 站点指针，如果不存在返回NULL
 */
static const Station* find_station_by_id(int station_id)
{
    for (int line = 0; line < 4; line++) {
        for (int i = 0; i < metro_lines[line].count; i++) {
            if (metro_lines[line].stations[i].id == station_id) {
                return &metro_lines[line].stations[i];
            }
        }
    }
    return NULL;
}

/**
 * @brief 获取两个相邻站点之间的距离
 * @param station_id1 站点1 ID
 * @param station_id2 站点2 ID
 * @param line_number 线路号
 * @return 距离，1表示相邻，-1表示不相邻
 */
static int get_adjacent_distance(int station_id1, int station_id2, int line_number)
{
    for (int i = 0; i < metro_lines[line_number - 1].count - 1; i++) {
        if (metro_lines[line_number - 1].stations[i].id == station_id1 &&
            metro_lines[line_number - 1].stations[i + 1].id == station_id2) {
            return 1;
        }
        if (metro_lines[line_number - 1].stations[i].id == station_id2 &&
            metro_lines[line_number - 1].stations[i + 1].id == station_id1) {
            return 1;
        }
    }
    return -1;
}

/**
 * @brief 获取站点包含的所有线路
 * @param station_id 站点ID
 * @param lines 输出线路数组
 * @return 线路数量
 */
static int get_station_lines(int station_id, int* lines)
{
    int count = 0;

    // 遍历所有线路，找出包含该站点的线路
    for (int line = 0; line < 4; line++) {
        for (int i = 0; i < metro_lines[line].count; i++) {
            if (metro_lines[line].stations[i].id == station_id) {
                lines[count++] = line + 1; // 线路号从1开始
                break; // 避免重复添加
            }
        }
    }

    return count;
}

/**
 * @brief BFS算法查找路径
 * @param start_id 起点站点ID
 * @param end_id 终点站点ID
 * @param output_route 输出路径
 */
static void bfs_find_path(int start_id, int end_id, Route* output_route)
{
    clear_search_state();

    // 获取起点和终点的所有线路
    int start_lines[4], end_lines[4];
    int start_line_count = get_station_lines(start_id, start_lines);
    int end_line_count = get_station_lines(end_id, end_lines);

    if (start_line_count == 0 || end_line_count == 0) {
        return; // 无效的站点
    }

    // 使用队列进行BFS搜索
    int queue[MAX_STATIONS * 4];
    int front = 0, rear = 0;

    // 将起点的所有线路加入队列
    for (int i = 0; i < start_line_count; i++) {
        int line = start_lines[i];
        int node_index = path_node_count;

        path_nodes[node_index].station_id = start_id;
        path_nodes[node_index].line_id = line;
        path_nodes[node_index].distance = 0;
        path_nodes[node_index].transfers = 0;
        path_nodes[node_index].parent_index = -1;

        queue[rear++] = node_index;
        path_node_count++;
    }

    // BFS搜索
    while (front < rear) {
        int current_index = queue[front++];
        PathNode* current = &path_nodes[current_index];

        // 检查是否到达终点
        if (current->station_id == end_id) {
            // 回溯路径
            int path_indices[MAX_PATH_LENGTH];
            int path_count = 0;
            int current_temp = current_index;

            while (current_temp != -1 && path_count < MAX_PATH_LENGTH) {
                path_indices[path_count++] = current_temp;
                current_temp = path_nodes[current_temp].parent_index;
            }

            // 填充路径结果
            output_route->step_count = 0;
            output_route->total_stations = path_count;
            output_route->distance = current->distance;
            output_route->transfer_count = current->transfers;

            // 填充路径步骤
            for (int i = path_count - 1; i >= 0 && output_route->step_count < MAX_PATH_LENGTH; i--) {
                PathNode* node = &path_nodes[path_indices[i]];
                const Station* station = find_station_by_id(node->station_id);

                if (station) {
                    output_route->steps[output_route->step_count].station_name = (char*)station->name;
                    output_route->steps[output_route->step_count].line_number = node->line_id;
                    output_route->steps[output_route->step_count].is_transfer = station->is_transfer > 0;

                    // 确定动作类型
                    if (i == path_count - 1) {
                        output_route->steps[output_route->step_count].action = 0; // 上车
                    } else if (i == 0) {
                        output_route->steps[output_route->step_count].action = 1; // 下车
                    } else {
                        PathNode* prev_node = NULL;
                        PathNode* next_node = NULL;

                        if (i > 0) {
                            prev_node = &path_nodes[path_indices[i - 1]];
                        }
                        if (i < path_count - 1) {
                            next_node = &path_nodes[path_indices[i + 1]];
                        }

                        if ((prev_node && node->line_id != prev_node->line_id) || (next_node && node->line_id != next_node->line_id)) {
                            output_route->steps[output_route->step_count].action = 2; // 换乘
                        } else {
                            output_route->steps[output_route->step_count].action = 3; // 经过
                        }
                    }

                    output_route->step_count++;
                }
            }

            return; // 找到路径，返回
        }

        // 搜索相邻站点
        for (int line = 0; line < 4; line++) {
            const MetroLine* metro_line = &metro_lines[line];

            // 查找当前站点在当前线路上的位置
            int current_pos = -1;
            for (int i = 0; i < metro_line->count; i++) {
                if (metro_line->stations[i].id == current->station_id) {
                    current_pos = i;
                    break;
                }
            }

            if (current_pos == -1) continue;

            // 搜索前一个和后一个站点
            for (int direction = -1; direction <= 1; direction += 2) {
                int next_pos = current_pos + direction;

                if (next_pos >= 0 && next_pos < metro_line->count) {
                    int next_station_id = metro_line->stations[next_pos].id;

                    // 创建新节点
                    int new_node_index = path_node_count;
                    PathNode* new_node = &path_nodes[new_node_index];

                    new_node->station_id = next_station_id;
                    new_node->line_id = line + 1;
                    new_node->distance = current->distance + 1;
                    new_node->transfers = current->transfers;
                    new_node->parent_index = current_index;

                    // 检查是否需要换乘
                    const Station* next_station = find_station_by_id(next_station_id);
                    const Station* current_station = find_station_by_id(current->station_id);

                    if (current_station && current_station->is_transfer > 0) {
                        // 检查换乘
                        if (line + 1 != current->line_id) {
                            new_node->transfers++;
                        }
                    }

                    queue[rear++] = new_node_index;
                    path_node_count++;
                }
            }
        }

        // 检查是否可以换乘到其他线路
        const Station* current_station = find_station_by_id(current->station_id);
        if (current_station && current_station->is_transfer > 0) {
            int transfer_line = current_station->is_transfer;

            if (transfer_line > 0 && transfer_line <= 4) {
                // 创建换乘节点
                int new_node_index = path_node_count;
                PathNode* new_node = &path_nodes[new_node_index];

                new_node->station_id = current->station_id;
                new_node->line_id = transfer_line;
                new_node->distance = current->distance + 1;
                new_node->transfers = current->transfers + 1;
                new_node->parent_index = current_index;

                queue[rear++] = new_node_index;
                path_node_count++;
            }
        }
    }
}

// 公共函数实现
void route_planner_init(void)
{
    clear_search_state();
}

RouteResult find_route(const char* start_name, const char* end_name)
{
    RouteResult result;
    memset(&result, 0, sizeof(RouteResult));

    const Station* start_station = find_station_by_name(start_name);
    const Station* end_station = find_station_by_name(end_name);

    if (!start_station || !end_station) {
        return result; // 站点不存在
    }

    // 查找路径
    bfs_find_path(start_station->id, end_station->id, &result.routes[0]);

    if (result.routes[0].step_count > 0) {
        result.route_count = 1;
    }

    return result;
}

Route find_best_route(const char* start_name, const char* end_name)
{
    return find_route(start_name, end_name).routes[0];
}

RouteResult find_route_by_station(const Station* start_station, const Station* end_station)
{
    RouteResult result;
    memset(&result, 0, sizeof(RouteResult));

    if (!start_station || !end_station) {
        return result;
    }

    bfs_find_path(start_station->id, end_station->id, &result.routes[0]);

    if (result.routes[0].step_count > 0) {
        result.route_count = 1;
    }

    return result;
}

void clear_route_result(RouteResult* result)
{
    if (result) {
        memset(result, 0, sizeof(RouteResult));
    }
}

void print_route(const Route* route)
{
    if (!route || route->step_count == 0) {
        printf("No route found.\n");
        return;
    }

    printf("Route: %d stations, %d transfers, distance: %d\n",
           route->total_stations, route->transfer_count, route->distance);

    for (int i = 0; i < route->step_count; i++) {
        const char* action_str = "";
        switch (route->steps[i].action) {
            case 0: action_str = "Board"; break;
            case 1: action_str = "Get off"; break;
            case 2: action_str = "Transfer"; break;
            case 3: action_str = "Pass"; break;
        }

        printf("%d: %s at %s (Line %d)%s\n",
               i + 1, action_str, route->steps[i].station_name, route->steps[i].line_number,
               route->steps[i].is_transfer ? " [Transfer]" : "");
    }
}

void get_route_description(const Route* route, char* buffer, int buffer_size)
{
    if (!route || !buffer || buffer_size == 0) {
        return;
    }

    if (route->step_count == 0) {
        snprintf(buffer, buffer_size, "未找到路径");
        return;
    }

    int offset = 0;

    // 添加概览信息
    offset += snprintf(buffer + offset, buffer_size - offset,
                     "共%d站，换乘%d次，距离%d站\n",
                     route->total_stations, route->transfer_count, route->distance);

    // 添加详细路线
    int current_line = -1;
    for (int i = 0; i < route->step_count && offset < buffer_size - 1; i++) {
        const RouteStep* step = &route->steps[i];

        if (step->action == 0) { // 上车
            offset += snprintf(buffer + offset, buffer_size - offset,
                             "从%s乘%d号线", step->station_name, step->line_number);
            current_line = step->line_number;
        } else if (step->action == 2) { // 换乘
            offset += snprintf(buffer + offset, buffer_size - offset,
                             "，在%s换乘%d号线", step->station_name, step->line_number);
            current_line = step->line_number;
        } else if (step->action == 1) { // 下车
            offset += snprintf(buffer + offset, buffer_size - offset,
                             "，到达%s", step->station_name);
        }
    }

    offset += snprintf(buffer + offset, buffer_size - offset, "。");
}
