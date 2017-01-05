#include "ksum.h"
#include<string.h>
#include<vector>
#include<unordered_map>
using namespace std;

/* For k-sum(k>2) problem, The lower bound and corresponding algorithm was given in the following link.
 * http://www.sigmainfy.com/blog/k-sum-problem-analysis-recursive-implementation-lower-bound.html
 * http://cs.stackexchange.com/questions/2973/generalised-3sum-k-sum-problem
 * For even k, lower bould is n^(k/2)log(n); For odd k, lower bound is n^((k+1)/2); however it takes n^(k/2) space in both
 * cases.
 */

void Table::Shrink(){
    int i=0, j=0;
    while(j<_count) {
        while(j<_count&&_data[j].value) {
            i++;
            j++;
        }
        for(; j<_count&&!_data[j].value; j++);
        for(; j<_count&&_data[j].value; i++,j++) {
            _data[i] = _data[j];
            _data[j].value = 0;
        }
    }
    if(i&&i!=_count) _count = i;
}

Table::Table() {
    memset(_data, 0, sizeof(_data));
    _count = 0;
    _firstNoMatch = 0;
}

void Table::Init() {
    int i = 0;
    const size_t len = 16;
    char buf[len];
    const char* sep = " ";
    while(fgets(buf, len-1, stdin)) {
        int len = strlen(buf);
        if(buf[0]=='\n') break;

        Record r;
        buf[len-1] = '\0';
        char* p = strtok(buf, sep);
        r.id = atoi(p);
        p = strtok(NULL, sep);
        r.value = atoi(p);
        r.match = NULL;
        AddRecord(r);
    }
}

size_t Table::Size() { return _count; }

void Table::AddRecord(Record r) { _data[_count++] = r; }

void Table::AddMatch(size_t index, Record* match) { 
    _data[index].match = match; 
    if(index != _firstNoMatch) {
        Record t = _data[index];
        _data[index] = _data[_firstNoMatch];
        _data[_firstNoMatch] = t;
    }
    _firstNoMatch++;
}

size_t Table::FirstNoMatch() { return _firstNoMatch; }

int Table::GetValue(size_t index) { return _data[index].value; }
Record Table::GetRecord(size_t index) { return _data[index]; }

void Table::Clear(int index[], size_t count) {
    for(int i=0; i<count; i++) _data[index[i]].value = 0;
    Shrink();
}

int Table::cmp(const void* s, const void* t) { return ((Record*)s)->value - ((Record*)t)->value; }

void Table::Sort() { qsort(_data, _count, sizeof(Record), cmp); }

int Table::Search(int target, int beg, int end) {
    Record t;
    t.value = target;
    Record* ret = (Record*)bsearch(&t, _data+beg, end-beg, sizeof(Record), cmp);
    if(ret) return ret-_data;
    return -1;
}

void Table::Print() {
    printf("\nRecords: count %lu\n", _count);
    for(int i=0; i<_count; i++) {
        printf("%d, %d\n", _data[i].id, _data[i].value);
    }
}

void Table::ShowMatches() {
    for(int i=0; i<_firstNoMatch; i++) {
        Record r = _data[i];
        if(r.match) {
            printf("Match success: [%d]%d = ", r.id, r.value);
            Record* p = r.match;
            while(p) {
                printf("[%d]%d ", p->id, p->value);
                p = p->match;
                if(p) printf("+ ");
            }
        }
        printf("\n");
    }
}

//for test
void Table::DoSearch(int target) {
    int index = Search(target, 0, _count);
    if(index>0) printf("%d hit\n ", GetValue(index));
    else printf("%d failed\n", target);
}

void Table::MakeHole(int k) { _data[k].value = 0; }


RecordBuffer::RecordBuffer() { _size = 0; }

Record* RecordBuffer::GetByCount(int count) {
    if(count<=0) return NULL;
    Record* p = _buffer+_size; 
    Record* saved = p;
    p->match = NULL;

    _size += count; //overflow
    while(--count) {
        p->match = p+1;
        p = p+1;
        p->match = NULL;
    }
    return saved;
}


// log(n)
Record* KSum::_1sum(int target, Table &t) {
    //buggy in search
    int position = t.Search(target, 0, t.Size());
    if (position<0) return NULL;

    Record* rp = _buffer.GetByCount(1);
    t.GetRecord(position).CopyTo(rp);

    _index[0] = position;
    t.Clear(_index, 1);

    return rp;
}

// n
Record* KSum::_2sum(int target, Table &t) {
    size_t beg = 0, end = t.Size()-1;
    while(beg<end) {
        int a = t.GetValue(beg), b = t.GetValue(end);
        if(a+b > target) end--;
        else if(a+b < target) beg++;
        else {
            Record* p = _buffer.GetByCount(2);
            t.GetRecord(beg).CopyTo(p);
            t.GetRecord(end).CopyTo(p+1);

            _index[0] = beg;
            _index[1] = end;
            t.Clear(_index, 2);

            return p;
        }
    }
    return NULL;
}

// n^2
Record* KSum::_3sum(int target, Table &t) {
    int i = 0;
    int a, b, c;
    for(;i<t.Size(); i++) {
        a = t.GetValue(i) - target;
        if(a>0) return NULL;

        int start = i+1, end = t.Size()-1;
        while(start < end) {
            b = t.GetValue(start);
            c = t.GetValue(end);
            if (a+b+c>0) end -= 1;
            else if(a+b+c<0) start += 1;
            else {
                Record* p = _buffer.GetByCount(3);
                t.GetRecord(i).CopyTo(p);
                t.GetRecord(start).CopyTo(p+1);
                t.GetRecord(end).CopyTo(p+2);

                _index[0] = i;
                _index[1] = start;
                _index[2] = end;
                t.Clear(_index, 3);

                return p;
            }
        }
    }
    return NULL;
};

Record* KSum::_4sum(int target, Table &t) { return _4sum(target, t, 0); }

Record* KSum::_5sum(int target, Table &t) { return _5sum(target, t, 0); }

// ~n^3
Record* KSum::_5sum(int target, Table &t, int beg) {
    for(int i=0; i<t.Size(); i++) {
        int value = target - t.GetValue(i);
        if(value<0) return NULL;
        Record* p = _4sum(value, t, i+1);
        if(p) {
            Record* q = _buffer.GetByCount(1);
            *q = t.GetRecord(i);
            q->match = p;

            _index[0] = i;
            t.Clear(_index, 1);

            return q;
        }
    }
}

// ~n^2 
// There is a n^2log(n) algorithm: get all pair-sums for a set of integers, then sort it, and finally search as _1sum do(need to handle the situation where two pairs share the same record).
Record* KSum::_4sum(int target, Table &t, int beg) {
    int n = t.Size();
    unordered_map<int, vector<pair<int, int> > >pairs;
    pairs.reserve(n*n);

    for(int i=beg; i<n; i++)
        for(int j = i+1 ; j < n; j++)
            pairs[t.GetValue(i)+t.GetValue(j)].push_back(make_pair(i,j));

    for(int i=beg; i<n-3; i++)
    {
        if(i != 0 && t.GetValue(i)== t.GetValue(i-1)) continue;
        for(int j=i+1; j<n-2; j++)
        {
            if(j != i+1 && t.GetValue(j) == t.GetValue(j-1)) continue;
            if(target <t.GetValue(i)+t.GetValue(j)) continue;
            if(pairs.find(target - t.GetValue(i) - t.GetValue(j)) != pairs.end())
            {
                vector<pair<int, int> > &sum2 = pairs[target - t.GetValue(i) - t.GetValue(j)];
                for(int k = 0; k < sum2.size(); k++)
                {
                    if(sum2[k].first <= j)continue; //make each element of 4-tuple different

                    _index[0] = i;
                    _index[1] = j;
                    _index[2] = sum2[k].first;
                    _index[3] = sum2[k].second;

                    Record* p = _buffer.GetByCount(4);
                    for(size_t z=0; z<4; z++) {
                        t.GetRecord(_index[z]).CopyTo(p+z);
                    }
                    t.Clear(_index, 4);

                    return p;
                }
            }
        }
    }
    return NULL;
}

KSum::KSum(Table& a, Table& b): a(a), b(b) { }

void KSum::DoMatch() {
    a.Sort();
    b.Sort();

    typedef Record* (KSum::*KSumPointer)(int, Table&);
    KSumPointer func[] = {&KSum::_1sum, &KSum::_2sum, &KSum::_3sum, &KSum::_4sum};
    size_t len = sizeof(func)/sizeof(KSumPointer);
    for(size_t i=0; i<len; i++) {
        for(size_t j=a.FirstNoMatch(); j<a.Size(); j++) {
            Record* ret = (this->*func[i])(a.GetValue(j), b);
            if(ret) a.AddMatch(j, ret);
        }
    }
}
