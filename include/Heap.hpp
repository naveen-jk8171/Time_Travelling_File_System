#ifndef HEAP_HPP
#define HEAP_HPP

#include <vector>
#include <ctime>
#include "File.hpp"

class File;

template <typename T>
class Heap{
private:
    std::vector<std::pair<File*, T>> heap;
    HashMap<std::string, int> m;
public:
    Heap(){}
    ~Heap(){}
    void tickleUp(int i){
        while(i > 0){
            int parent = (i-1)/2;
            if (heap[i].second > heap[parent].second){
                std::swap(heap[parent], heap[i]);
                m.insertItem(heap[parent].first->getFileName(), parent);
                m.insertItem(heap[i].first->getFileName(), i);
                i = parent;
            }else{
                break;
            }
        }
    }
    void tickleDown(int i){
        while(i < heap.size()){
            int left = 2*i+1, right = 2*i+2, maxEle = i;
            if (left < heap.size() && heap[left].second > heap[maxEle].second) maxEle = left;
            if (right < heap.size() && heap[right].second > heap[maxEle].second) maxEle = right;
            if (maxEle == i) break;
            std::swap(heap[maxEle], heap[i]);
            m.insertItem(heap[maxEle].first->getFileName(), maxEle);
            m.insertItem(heap[i].first->getFileName(), i);
            i = maxEle;
        }
    }
    void addFile(File* file, T value){
        heap.push_back({file, value});
        m.insertItem(file->getFileName(), heap.size()-1);
        tickleUp(heap.size()-1);
    }
    std::pair<File*, T> popFile(){
        std::pair<File*, T> ans = heap[0];
        std::swap(heap[0], heap[heap.size()-1]);
        m.removeItem(heap[heap.size()-1].first->getFileName());
        m.insertItem(heap[0].first->getFileName(), 0);
        heap.pop_back();
        tickleDown(0);
        return ans;
    };
    int findInd(File* file){
        if (!m.contains(file->getFileName())) return -1;
        return m.searchTable(file->getFileName());
    }
    void update(int ind, T value){
        heap[ind].second = value;
        tickleUp(ind);
        tickleDown(ind);
    }
    int getHeapSize(){
        return heap.size();
    }
};

#endif