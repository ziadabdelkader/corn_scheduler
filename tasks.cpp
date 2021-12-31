#include "tasks.h"
#include <fstream>
#include <chrono>

void write_to_file(const std::string& path, const std::string& message){
    std::ofstream file;
    file.open(path, std::ios_base::app); //appending to the file content
    file << message;
    file.close();
}

void write_many(const std::string& path, int32_t count){
    std::ofstream file;
    file.open(path, std::ios_base::app); //appending to the file content
    for(int i=0 ; i< count ; i++){
        file << "line number " << i << "\n";
    }
    file.close();
}

void mod_many(const std::string& path, int32_t count){
    std::ofstream file;
    file.open(path, std::ios_base::app); //appending to the file content
    long long res = 0;
    for(int i=0 ; i< count ; i++){
        res += rand() % rand();
    }
    file << res << "\n";
    file.close();
}

void failing(){
    throw std::exception();
}