#ifndef KSUM_H_
#define KSUM_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN (1024*16)

struct Record {
    int id;
    int value;
    Record* match;
    void CopyTo(Record* p) {
        p->id = id;
        p->value = value;
        // printf("Value: %d\n", value);
    };
};

class Table {
    private:
        Record _data[MAX_LEN];
        size_t _count;
        size_t _firstNoMatch;
        void Shrink();
        static int cmp(const void*, const void*);
    public:
        Table();
        void Init();
        size_t Size();
        size_t FirstNoMatch();
        void AddRecord(Record);
        void AddMatch(size_t index, Record*);
        int GetValue(size_t index);
        Record GetRecord(size_t index);

        void Clear(int index[], size_t count);
        void Sort();
        void Print();
        int Search(int, int, int);
        void ShowMatches();

        // for test
        void DoSearch(int);
        void MakeHole(int);
};

class RecordBuffer {
    public:
        RecordBuffer();
        Record* GetByCount(int count);
    private:
        Record _buffer[MAX_LEN];
        size_t _size;
};

class KSum {
    public:
        KSum(Table& a, Table& b);
        void DoMatch();
    private:
        Table &a, &b;
        RecordBuffer _buffer;
        int _index[256];
        Record* _1sum(int target, Table &t);
        Record* _2sum(int target, Table &t);
        Record* _3sum(int target, Table &t);
        Record* _4sum(int target, Table &t);
        Record* _5sum(int target, Table &t);
        Record* _4sum(int target, Table &t, int beg);
        Record* _5sum(int target, Table &t, int beg);
};

#endif
