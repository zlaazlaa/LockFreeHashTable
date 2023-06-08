#include<unordered_map>
#include "iostream"
#include <vector>
#include "LRUCache_single.h"
#include "create_zipfian_data.h"
#include <string>
#include <cstdio>
#include <random>
#include <chrono>
using namespace std;
using key_type = string;
using value_type = string;
value_type error_info = "-1";
bool operator!=(const value_type& lhs, const value_type& rhs) {
    return !(lhs == rhs);
}

const int key_range = 1000; // [0, key_range] // Corpus size
const int job_num = 1000000;
const int client_num = 1;
const float read_rate = 0.8;
const int cache_capacity = 100;
const int key_len = 8;
const int value_len = 16;
int hit_sum, get_sum;
struct Task {
    key_type key;
    value_type value;
    bool job_type;
};
Task task[job_num];
value_type keys[job_num], values[job_num];

void generate_zipfain_data() {
    int numStrings = key_range;  // 总共的字符串数量
    double zipfianAlpha = 1.0;  // Zipfian 分布的参数
    ZipfianStringGenerator generator_key(numStrings, zipfianAlpha, key_len);
    ZipfianStringGenerator generator_value(numStrings, zipfianAlpha, value_len);

    int testCount = job_num;
    for (int i = 0; i < testCount; ++i) {
        std::string randomString = generator_key.getRandomString();
        keys[i] = randomString;
        // keys.push_back(randomString);
    }

    for (int i = 0; i < testCount; ++i) {
        std::string randomString = generator_value.getRandomString();
        values[i] = randomString;
        // values.push_back(randomString);
    }
}

int main() {
    generate_zipfain_data();
    LRUCache<key_type, value_type>* lRUCache = new LRUCache<key_type, value_type>(cache_capacity);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, key_range);
    std::uniform_real_distribution<> dis2(0, 1);
    for (int i = 0; i < job_num; i++) {
        task[i] = {keys[i], values[i], (dis2(gen) > read_rate) ? false : true}; // false -> put, true -> get;
    }
    printf("任务生成完毕, 线程数:%d, 任务总量:%d, 读操作比重:%f\n", client_num, job_num, read_rate);
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0;i < job_num;i ++) {
        Task now = task[i];
        if (now.job_type) { // get
            if (lRUCache->get(now.key) != error_info) {
                hit_sum++;
            }
            get_sum++;
        } else {
            lRUCache->put(now.key, now.value);
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    cout << "任务执行完毕" << endl;
    std::cout << "程序运行时间：" << duration.count() << " 微秒" << std::endl;
    std::cout << "总get:" << get_sum << ", 命中次数:" << hit_sum << ", 命中率:" << (float)hit_sum/get_sum << endl;
}
/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */