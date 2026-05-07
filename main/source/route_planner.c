#include "route_planner.h"
#include <string.h>
#include <stdlib.h>

extern const MetroLine metro_lines[4];

// 简化的图节点结构
typedef struct {
    int station_id;
    int line_id;
    int distance;
    int transfers; //累计换乘次数
    int parent_index;
} PathNode;

// 全局变量
static PathNode path_nodes[MAX_STATIONS * 4]; // 存储所有搜索过的节点
static int path_node_count = 0; // 当前节点数量
static int path_parent[MAX_STATIONS * 4]; // 用于回溯路径
static int path_line[MAX_STATIONS * 4];   // 线路信息
static int path_distance[MAX_STATIONS * 4]; // 距离信息
static bool visited[MAX_STATIONS][5]; // 访问标记：visited[station_id][line_id]

/**
 * @brief 清空路径搜索状态
 */
static void clear_search_state(void)
{
    path_node_count = 0;
    memset(path_parent, -1, sizeof(path_parent));
    memset(path_line, -1, sizeof(path_line));
    memset(path_distance, -1, sizeof(path_distance));
    memset(visited, false, sizeof(visited));
}

/**
 * @brief 根据站点拼音查找站点指针
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
            if (strcmp(metro_lines[line].stations[i].name_pinyin, station_name) == 0) {
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
            if (metro_lines[line].stations[i].only_id == station_id) {
                return &metro_lines[line].stations[i];
            }
        }
    }
    return NULL;
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
            if (metro_lines[line].stations[i].only_id == station_id) {
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

        if (path_node_count >= MAX_STATIONS * 4 - 1) {
            return;
        }

        visited[start_id][line] = true;
        int node_index = path_node_count++;

        path_nodes[node_index].station_id = start_id; //only_id
        path_nodes[node_index].line_id = line;
        path_nodes[node_index].distance = 0;
        path_nodes[node_index].transfers = 0;
        path_nodes[node_index].parent_index = -1;

        queue[rear++] = node_index; //path_node_count = 1;node_index = 0;rear = 1;front = 0;
    }

    // BFS搜索，设置最大搜索次数防止死循环
    int max_searches = 500;
    int search_count = 0;

    while (front < rear && search_count < max_searches) {
        search_count++;
        int current_index = queue[front++]; //cureent_index = 0;front = 1;
        PathNode* current = &path_nodes[current_index];

        // 检查是否到达终点
        if (current->station_id == end_id) {
            // 回溯路径
            int path_indices[MAX_PATH_LENGTH];  // 存储路径上的节点索引
            int path_count = 0;  //记录路径上的节点总数
            int current_temp = current_index;  //从终点开始回溯到起点，记录路径上的节点索引

            while (current_temp != -1 && path_count < MAX_PATH_LENGTH) {
                path_indices[path_count++] = current_temp;
                current_temp = path_nodes[current_temp].parent_index;
            }

            // 填充output_route路径结果
            output_route->step_count = 0;
            output_route->distance = current->distance;
            output_route->transfer_count = current->transfers;

            // 填充路径步骤
            for (int i = path_count - 1; i >= 0 && output_route->step_count < MAX_PATH_LENGTH; i--) {
                PathNode* node = &path_nodes[path_indices[i]];
                const Station* station = find_station_by_id(node->station_id);

                // 跳过重复的换乘站点
                if (i < path_count - 1 && station) {
                    PathNode* prev_node = &path_nodes[path_indices[i + 1]];
                    const Station* prev_station = find_station_by_id(prev_node->station_id);
                    if (prev_station && station->only_id == prev_station->only_id) {
                        continue;  // 跳过重复的换乘站点
                    }
                }

                if (station) {
                    output_route->steps[output_route->step_count].station_name = (char*)station->name_pinyin;
                    output_route->steps[output_route->step_count].line_number = node->line_id;
                    output_route->steps[output_route->step_count].is_transfer = station->is_transfer > 0;

                    // 确定动作类型
                    if (i == path_count - 1) {
                        output_route->steps[output_route->step_count].action = 0;  //上车
                    } else if (i == 0) {
                        output_route->steps[output_route->step_count].action = 1;  //下车
                    } else {
                        PathNode* prev_node = &path_nodes[path_indices[i - 1]];

                        // 简化换乘判断逻辑：只检查与前一个节点的线路变化
                        if (prev_node && node->line_id != prev_node->line_id) {
                            output_route->steps[output_route->step_count].action = 2;  //换乘
                        } else {
                            output_route->steps[output_route->step_count].action = 3;
                        }
                    }

                    output_route->step_count++;
                }
            }

            // 修正total_stations为实际步骤数（去重后）
            output_route->total_stations = output_route->step_count;

            return;
        }

        // 搜索相邻站点（只在当前线路）
        const MetroLine* metro_line = &metro_lines[current->line_id - 1];

        // 查找当前站点位置
        int current_pos = -1;
        for (int i = 0; i < metro_line->count; i++) {
            if (metro_line->stations[i].only_id == current->station_id) {
                current_pos = i;
                break;
            }
        }

        if (current_pos >= 0) {
            // 搜索前后站点
            for (int direction = -1; direction <= 1; direction += 2) {
                int next_pos = current_pos + direction;

                if (next_pos >= 0 && next_pos < metro_line->count) {
                    int next_station_id = metro_line->stations[next_pos].only_id;

                    if (visited[next_station_id][current->line_id]) {
                        continue;
                    }

                    if (path_node_count >= MAX_STATIONS * 4 - 1) {
                        return;
                    }

                    visited[next_station_id][current->line_id] = true;
                    int new_node_index = path_node_count++;

                    path_nodes[new_node_index].station_id = next_station_id;
                    path_nodes[new_node_index].line_id = current->line_id;
                    path_nodes[new_node_index].distance = current->distance + 1;
                    path_nodes[new_node_index].transfers = current->transfers;
                    path_nodes[new_node_index].parent_index = current_index;

                    queue[rear++] = new_node_index;
                }
            }
        }

        // 检查换乘
        int station_lines[4];
        int line_count = get_station_lines(current->station_id, station_lines);

        for (int i = 0; i < line_count; i++) {
            int transfer_line = station_lines[i];

            if (transfer_line == current->line_id) {
                continue;
            }

            if (visited[current->station_id][transfer_line]) {
                continue;
            }

            if (path_node_count >= MAX_STATIONS * 4 - 1) {
                return;
            }

            visited[current->station_id][transfer_line] = true;
            int new_node_index = path_node_count++;

            path_nodes[new_node_index].station_id = current->station_id;
            path_nodes[new_node_index].line_id = transfer_line;
            path_nodes[new_node_index].distance = current->distance;
            path_nodes[new_node_index].transfers = current->transfers + 1;
            path_nodes[new_node_index].parent_index = current_index;

            queue[rear++] = new_node_index;
        }
    }
}

// 公共函数实现
void find_route(const char* start_name, const char* end_name, Route* output_route)
{
    memset(output_route, 0, sizeof(Route));

    // 检查输入是否为空
    if (!start_name || !end_name || strlen(start_name) == 0 || strlen(end_name) == 0) {
        return;
    }

    const Station* start_station = find_station_by_name(start_name);
    const Station* end_station = find_station_by_name(end_name);

    if (!start_station || !end_station) {
        return; // 站点不存在
    }

    // 检查起点和终点是否相同
    if (start_station->only_id == end_station->only_id) {
        return; // 起点和终点相同
    }

    // 查找路径
    bfs_find_path(start_station->only_id, end_station->only_id, output_route);

}