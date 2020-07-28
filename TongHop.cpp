#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <chrono>
#include <map>
#include <dirent.h>
#include <sys/types.h>
using namespace std;
using namespace std::chrono;

//General
class List
{
public:
    string file01;
    string file02;
    int KQ;
    List() {};
    List(string, string, int);
    void Xuat();
};

List::List(string f01, string f02, int kq)
{
    file01 = f01;
    file02 = f02;
    KQ = kq;
}

void List::Xuat()
{
    size_t pos01 = file01.find("\sm_doc_set");
    size_t pos02 = file02.find("\sm_doc_set");
    string str01 = file01.substr (pos01);
    string str02 = file02.substr (pos02);
    cout << str01 << " && " << str02 << " \t " << KQ << endl;
}

void Select_folder(string folder, vector<string> &files)
{
    DIR *tm;
    struct dirent *link_folder;
    if((tm  = opendir(folder.c_str())) == NULL)
    {
        cout << "Error!";
        return;
    }
    while ((link_folder = readdir(tm)) != NULL)
    {
        string link_file = string(link_folder->d_name);
        if (link_file.find(".txt") != -1)
            files.push_back(folder + link_file);
    }
    closedir(tm);
}

void splitString(vector<string>& CH, string data)
{
    char *s = const_cast<char *> (data.c_str());
    char * b;
    b = strtok (s," .,:;`'/+-(){}[]<>*&^%$#@!?~/\\= \r\t\n1234567890");
    while (b != NULL)
    {
        CH.push_back(b);
        b = strtok(NULL, " .,:;`'/+-(){}[]<>*&^%$#@!?~/\\= \r\t\n1234567890");
    }
}

void push_vector_2_ngrams(vector<string> CH, vector<string> &twoCH)
{
    for (int i = 0; i < CH.size() - 1; i++)
    {
		string tmp;
		tmp = CH[i] + " " + CH[i + 1];
		twoCH.push_back(tmp);
	}
}

void push_vector_3_ngrams(vector<string> CH, vector<string> &threeCH)
{
    for (int i = 0; i < CH.size() - 2; i++)
    {
		string tmp;
		tmp = CH[i] + " " + CH[i + 1] + " " + CH[i + 2];
		threeCH.push_back(tmp);
	}
}

void readFile(string a, vector<string>& line)
{
    ifstream file(a);
    string in;
    if (file.is_open())
    {
        while ( getline(file, in) )
        {
            if (in != "")
            {
                line.push_back(in);
            }
        }
    }
}

void QuickSort(List A[], int l, int r)
{
    int i, j, X;
    X = A[(l+r)/2].KQ;
    i = l; j = r;
    while (i <= j)
    {
        while (A[i].KQ > X) i++;
        while (A[j].KQ < X) j--;
        if (i <= j)
        {
            List t = A[i];
            A[i] = A[j];
            A[j] = t;
            i++; j--;
        }
    }
    if (l < j) QuickSort(A, l, j);
    if (i < r) QuickSort(A, i, r);
}

//Binary Tree
struct node
{
    string data;
    int SoLuong;
    node* left;
    node* right;
};

void _insert(node*& tree, string info)
{
    if (!tree)
    {
        tree = new node;
        tree->data = info;
        tree->SoLuong = 1;
        tree->left = NULL;
        tree->right = NULL;
    }
    else
    {
        if (tree->data > info)
            _insert(tree->left, info);
        else if (tree->data < info)
            _insert(tree->right, info);
        else
        {
            tree->SoLuong++;
            return;
        }
    }
}

void push_1_gram(node*& tree, vector<string> line)
{
    vector<string> oneCH;
    for (int i = 0; i < line.size(); i++)
    {
        splitString(oneCH, line[i]);
    }

    for (int i = 0; i < oneCH.size(); i++)
    {
        _insert(tree, oneCH[i]);
    }
}

void push_2_gram(node*& tree, vector<string> line)
{
    vector<string> oneCH;
    for (int i = 0; i < line.size(); i++)
    {
        splitString(oneCH, line[i]);
    }
    vector<string> twoCH;
    push_vector_2_ngrams(oneCH, twoCH);

    for (int i = 0; i < twoCH.size(); i++)
    {
        _insert(tree, twoCH[i]);
    }
}

void push_3_gram(node*& tree, vector<string> line)
{
    vector<string> oneCH;
    for (int i = 0; i < line.size(); i++)
    {
        splitString(oneCH, line[i]);
    }
    vector<string> threeCH;
    push_vector_2_ngrams(oneCH, threeCH);

    for (int i = 0; i < threeCH.size(); i++)
    {
        _insert(tree, threeCH[i]);
    }
}

void count_bst_grams(node* tree01, node* tree02, int& dem)
{
    if(!tree01) return;
    node* temp = tree02;
    while(temp)
    {
        if (tree01->data == temp->data)
        {
            dem = dem + min(tree01->SoLuong, temp->SoLuong);
            break;
        }
        else if (tree01->data > temp->data)
            temp = temp->right;
        else
            temp = temp->left;
    }
    count_bst_grams(tree01->left, tree02, dem);
    count_bst_grams(tree01->right, tree02, dem);
}

int compare_bst_grams(string a, string b, string type)
{
    //file 1
    vector<string> line;
    readFile(a, line);
    node* OneGram01 = NULL;
    push_1_gram(OneGram01, line);

    //file 2
    vector<string> lines;
    readFile(b, lines);
    node* OneGram02 = NULL;
    push_1_gram(OneGram02, lines);
    int dem = 0;
    //re nhanh
    if (type == "1-0")       //1-0 gram
    {
        count_bst_grams(OneGram01, OneGram02, dem);
        return dem;
    }
    else if (type == "2-0")  //2-0 grams
    {
        node* TwoGram01 = NULL;
        push_2_gram(TwoGram01, line);

        node* TwoGram02 = NULL;
        push_2_gram(TwoGram02, lines);

        count_bst_grams(TwoGram01, TwoGram02, dem);
        return dem;
    }
    else if (type == "3-0")  //3-0 grams
    {
        node* ThreeGram01 = NULL;
        push_3_gram(ThreeGram01, line);

        node* ThreeGram02 = NULL;
        push_3_gram(ThreeGram02, lines);

        count_bst_grams(ThreeGram01, ThreeGram02, dem);
        return dem;
    }
    else if (type == "1-2")  //1-2 grams
    {
        node* TwoGram01 = NULL;
        push_2_gram(TwoGram01, line);

        node* TwoGram02 = NULL;
        push_2_gram(TwoGram02, lines);
        int dem02 = 0;
        count_bst_grams(OneGram01, OneGram02, dem);
        count_bst_grams(TwoGram01, TwoGram02, dem02);
        return dem + dem02;
    }
    else if (type == "1-3")  //1-3 grams
    {
        node* TwoGram01 = NULL;
        push_2_gram(TwoGram01, line);
        node* TwoGram02 = NULL;
        push_2_gram(TwoGram02, lines);

        node* ThreeGram01 = NULL;
        push_3_gram(ThreeGram01, line);
        node* ThreeGram02 = NULL;
        push_3_gram(ThreeGram02, lines);
        count_bst_grams(OneGram01, OneGram02, dem);
        count_bst_grams(TwoGram01, TwoGram02, dem);
        count_bst_grams(ThreeGram01, ThreeGram02, dem);
        return dem;
    }
    return 0;
}

int ListFile_bst(List A[], string link, string type)
{
    string folder = link;
    vector<string> files = vector<string>();
    Select_folder(folder, files);
    int n = 0;
    for(int i = 0; i < files.size(); i++)
    {
        for (int j = i+1 ; j < files.size(); j++)
        {
            A[n].KQ = compare_bst_grams(files[i],files[j], type);
            A[n].file01 = files[i];
            A[n].file02 = files[j];
            n++;
        }
    }
    return n;
}

//Vector
int count_vector_grams(vector<string> ch, vector<string> chs)
{
    int dem = 0;
    for (int i = 0; i < ch.size(); i++)
    {
        for (int j = 0; j < chs.size(); j++)
        {
            if (ch[i] == chs[j])
            {
                dem++;
                swap(chs[j], chs.back());
                chs.pop_back();
                break;
            }
        }
    }
    return dem;
}

int Compare_vector_grams(string a, string b, string type)
{
    ifstream file(a);
    ifstream files(b);
    string in;
    string inp;
    vector<string> lines;
    vector<string> liness;
    vector<string> OneCH;   //1 gram cua file 1
    vector<string> OneCHs;  //1 gram cua file 2

    //Doc file thu 1
    if (file.is_open())
    {
        while ( getline(file, in) )
        {
            lines.push_back(in);
        }
    }

    for (int i = 0; i < lines.size(); i++)
    {
        splitString(OneCH, lines[i]);
    }
    //Doc file thu 2
    if (files.is_open())
    {
        while ( getline(files, inp) )
        {
            liness.push_back(inp);
        }
    }
    for (int i = 0; i < liness.size(); i++)
    {
        splitString(OneCHs, liness[i]);
    }

    //Re nhanh cac truong hop
    if (type == "1-0")       //1-0 gram
        return count_vector_grams(OneCH, OneCHs);
    else if (type == "2-0")  //2-0 grams
    {
        //gram cua file 1
        vector<string> TwoCH;
        //gram cua file 2
        vector<string> TwoCHs;

        push_vector_2_ngrams(OneCH, TwoCH);
        push_vector_2_ngrams(OneCHs, TwoCHs);

        return count_vector_grams(TwoCH, TwoCHs);
    }
    else if (type == "3-0")  //3-0 grams
    {
        //gram cua file 1
        vector<string> TwoCH;
        vector<string> ThreeCH;
        //gram cua file 2
        vector<string> TwoCHs;
        vector<string> ThreeCHs;

        push_vector_2_ngrams(OneCH, TwoCH);
        push_vector_3_ngrams(OneCH, ThreeCH);
        push_vector_2_ngrams(OneCHs, TwoCHs);
        push_vector_3_ngrams(OneCHs, ThreeCHs);

        return count_vector_grams(ThreeCH, ThreeCHs);
    }
    else if (type == "1-2")  //1-2 grams
    {
        //gram cua file 1
        vector<string> TwoCH;
        //gram cua file 2
        vector<string> TwoCHs;

        push_vector_2_ngrams(OneCH, TwoCH);
        push_vector_2_ngrams(OneCHs, TwoCHs);

        return count_vector_grams(OneCH, OneCHs) + count_vector_grams(TwoCH, TwoCHs);
    }
    else if (type == "1-3")  //1-3 grams
    {
        //gram cua file 1
        vector<string> TwoCH;
        vector<string> ThreeCH;
        //gram cua file 2
        vector<string> TwoCHs;
        vector<string> ThreeCHs;

        push_vector_2_ngrams(OneCH, TwoCH);
        push_vector_3_ngrams(OneCH, ThreeCH);
        push_vector_2_ngrams(OneCHs, TwoCHs);
        push_vector_3_ngrams(OneCHs, ThreeCHs);

        return count_vector_grams(OneCH, OneCHs) + count_vector_grams(TwoCH, TwoCHs) + count_vector_grams(ThreeCH, ThreeCHs);
    }
    return 0;
}

int ListFile_vector(List A[], string link, string type)
{
    string folder = link;
    vector<string> files = vector<string>();
    Select_folder(folder, files);
    int n = 0;
    for(int i = 0; i < files.size(); i++)
    {
        for (int j = i+1 ; j < files.size(); j++)
        {
            A[n].KQ = Compare_vector_grams(files[i],files[j], type);
            A[n].file01 = files[i];
            A[n].file02 = files[j];
            n++;
        }
    }
    return n;
}

//Map
int count_map_grams(map<string,int> map01, map<string, int> map02)
{
    int dem = 0;
    map<string, int>::iterator it;
    map<string, int>::iterator itr;
    for (it = map01.begin(); it != map01.end(); it++)
    {
        itr = map02.find(it->first);
        if(itr != map02.end())
        {
            int saiso = max(it->second, itr->second) - min(it->second, itr->second);
            if (saiso > 0)
            {
                dem = dem + (max(it->second, itr->second) - saiso);
            }
            else
            {
                dem = dem + max(it->second, itr->second);
            }
        }
    }
    return dem;
}

map<string, int> AddMapGram(vector<string> CH)
{
    map<string, int> nGram;
    map<string, int>::iterator it;
    for (int i = 0; i < CH.size(); i++)
    {
        it = nGram.find(CH[i]);
        if (it != nGram.end())
            nGram[CH[i]]++;
        else
            nGram[CH[i]] = 1;
    }
    return nGram;
}

int compare_map_grams(string a, string b, string type)
{
    //file 1
    vector<string> line;
    readFile(a, line);
    vector<string> OneCH;   //1 gram cua file 1
    //Lay 1 gram
    for (int i = 0; i < line.size(); i++)
    {
        splitString(OneCH, line[i]);
    }
    map<string, int> OneGram;
    OneGram = AddMapGram(OneCH);

    //file 2
    vector<string> lines;
    readFile(b, lines);
    vector<string> OneCHs;   //1 gram cua file 2
    //Lay 1 gram
    for (int i = 0; i < lines.size(); i++)
    {
        splitString(OneCHs, lines[i]);
    }
    map<string, int> OneGrams;
    OneGrams = AddMapGram(OneCHs);

    //re nhanh
    if (type == "1-0")       //1-0 gram
        return count_map_grams(OneGram, OneGrams);
    else if (type == "2-0")  //2-0 grams
    {
        vector<string> TwoCH;
        push_vector_2_ngrams(OneCH, TwoCH);
        map<string, int> TwoGram;
        TwoGram = AddMapGram(TwoCH);

        vector<string> TwoCHs;
        push_vector_2_ngrams(OneCHs, TwoCHs);
        map<string, int> TwoGrams;
        TwoGrams = AddMapGram(TwoCHs);

        return count_map_grams(TwoGram, TwoGrams);
    }
    else if (type == "3-0")  //3-0 grams
    {
        vector<string> ThreeCH;
        push_vector_3_ngrams(OneCH, ThreeCH);
        map<string, int> ThreeGram;
        ThreeGram = AddMapGram(ThreeCH);

        vector<string> ThreeCHs;
        push_vector_3_ngrams(OneCHs, ThreeCHs);
        map<string, int> ThreeGrams;
        ThreeGrams = AddMapGram(ThreeCHs);

        return count_map_grams(ThreeGram, ThreeGrams);
    }
    else if (type == "1-2")  //1-2 grams
    {
        vector<string> TwoCH;
        push_vector_2_ngrams(OneCH, TwoCH);
        map<string, int> TwoGram;
        TwoGram = AddMapGram(TwoCH);

        vector<string> TwoCHs;
        push_vector_2_ngrams(OneCHs, TwoCHs);
        map<string, int> TwoGrams;
        TwoGrams = AddMapGram(TwoCHs);

        return count_map_grams(OneGram, OneGrams) + count_map_grams(TwoGram, TwoGrams);
    }
    else if (type == "1-3")  //1-3 grams
    {
        vector<string> TwoCH;
        push_vector_2_ngrams(OneCH, TwoCH);
        map<string, int> TwoGram;
        TwoGram = AddMapGram(TwoCH);

        vector<string> TwoCHs;
        push_vector_2_ngrams(OneCHs, TwoCHs);
        map<string, int> TwoGrams;
        TwoGrams = AddMapGram(TwoCHs);

        vector<string> ThreeCH;
        push_vector_3_ngrams(OneCH, ThreeCH);
        map<string, int> ThreeGram;
        ThreeGram = AddMapGram(ThreeCH);

        vector<string> ThreeCHs;
        push_vector_3_ngrams(OneCHs, ThreeCHs);
        map<string, int> ThreeGrams;
        ThreeGrams = AddMapGram(ThreeCHs);

        return count_map_grams(OneGram, OneGrams) + count_map_grams(TwoGram, TwoGrams) + count_map_grams(ThreeGram, ThreeGrams);
    }
    return 0;
}

int ListFile_map(List A[], string link, string type)
{
    string folder = link;
    vector<string> files = vector<string>();
    Select_folder(folder, files);
    int n = 0;
    for(int i = 0; i < files.size(); i++)
    {
        for (int j = i+1 ; j < files.size(); j++)
        {
            A[n].KQ = compare_map_grams(files[i],files[j], type);
            A[n].file01 = files[i];
            A[n].file02 = files[j];
            n++;
        }
    }
    return n;
}

//Map From Scratch
struct HashTable
{
    string key;
    int SoLuong;
};

int SearchKey(HashTable A[], int n, string text)
{
    for (int i = 0; i < n; i++)
    {
        if (A[i].key == text)
            return A[i].SoLuong;
    }
    return 0;
}

int SearchPosition(HashTable A[], int n, string text)
{
    for (int i = 0; i < n; i++)
    {
        if (A[i].key == text)
            return i;
    }
    return -1;
}

int AddHashGram(HashTable A[], vector<string> CH)
{
    int n = 0;
    for (int i = 0; i < CH.size(); i++)
    {
        int k = SearchPosition(A, i, CH[i]);
        if (k != -1)
        {
            A[k].SoLuong++;
        }
        else
        {
            A[n].key = CH[i];
            A[n].SoLuong = 1;
            n++;
        }
    }
    return n;
}

int count_hashTable(HashTable h01[], int n, HashTable h02[], int m)
{
    int dem = 0;
    for (int i = 0; i < n; i++)
    {
        int k = SearchKey(h02, m, h01[i].key);
        if (k != 0)
            dem = dem + min(k, h01[i].SoLuong);
    }
    return dem;
}

int compare_hashTable_grams(string a, string b, string type)
{
    //file 1
    vector<string> line;
    readFile(a, line);
    vector<string> OneCH;   //1 gram cua file 1
    //Lay 1 gram
    for (int i = 0; i < line.size(); i++)
    {
        splitString(OneCH, line[i]);
    }
    HashTable OneGram01[5000];
    int n01;
    n01 = AddHashGram(OneGram01, OneCH);

    //file 2
    vector<string> lines;
    readFile(b, lines);
    vector<string> OneCHs;   //1 gram cua file 2
    //Lay 1 gram
    for (int i = 0; i < lines.size(); i++)
    {
        splitString(OneCHs, lines[i]);
    }
    HashTable OneGram02[5000];
    int n02;
    n02 = AddHashGram(OneGram02, OneCHs);

    //re nhanh
    if (type == "1-0")       //1-0 gram
        return count_hashTable(OneGram01, n01, OneGram02, n02);
    else if (type == "2-0")  //2-0 grams
    {
        vector<string> TwoCH;
        push_vector_2_ngrams(OneCH, TwoCH);
        HashTable TwoGram01[5000];
        int m01;
        m01 = AddHashGram(TwoGram01, TwoCH);

        vector<string> TwoCHs;
        push_vector_2_ngrams(OneCHs, TwoCHs);
        HashTable TwoGram02[5000];
        int m02;
        m02 = AddHashGram(TwoGram02, TwoCHs);
        return count_hashTable(TwoGram01, m01, TwoGram02, m02);
    }
    else if (type == "3-0")  //3-0 grams
    {
        vector<string> ThreeCH;
        push_vector_3_ngrams(OneCH, ThreeCH);
        HashTable ThreeGram01[5000];
        int m01;
        m01 = AddHashGram(ThreeGram01, ThreeCH);

        vector<string> ThreeCHs;
        push_vector_3_ngrams(OneCHs, ThreeCHs);
        HashTable ThreeGram02[5000];
        int m02;
        m02 = AddHashGram(ThreeGram02, ThreeCHs);
        return count_hashTable(ThreeGram01, m01, ThreeGram02, m02);
    }
    else if (type == "1-2")  //1-2 grams
    {
        vector<string> TwoCH;
        push_vector_2_ngrams(OneCH, TwoCH);
        HashTable TwoGram01[5000];
        int m01;
        m01 = AddHashGram(TwoGram01, TwoCH);

        vector<string> TwoCHs;
        push_vector_2_ngrams(OneCHs, TwoCHs);
        HashTable TwoGram02[5000];
        int m02;
        m02 = AddHashGram(TwoGram02, TwoCHs);
        return count_hashTable(OneGram01, n01, OneGram02, n02) + count_hashTable(TwoGram01, m01, TwoGram02, m02);
    }
    else if (type == "1-3")  //1-3 grams
    {
        vector<string> TwoCH;
        push_vector_2_ngrams(OneCH, TwoCH);
        HashTable TwoGram01[5000];
        int m01;
        m01 = AddHashGram(TwoGram01, TwoCH);

        vector<string> TwoCHs;
        push_vector_2_ngrams(OneCHs, TwoCHs);
        HashTable TwoGram02[5000];
        int m02;
        m02 = AddHashGram(TwoGram02, TwoCHs);

        vector<string> ThreeCH;
        push_vector_3_ngrams(OneCH, ThreeCH);
        HashTable ThreeGram01[5000];
        int p01;
        p01 = AddHashGram(ThreeGram01, ThreeCH);

        vector<string> ThreeCHs;
        push_vector_3_ngrams(OneCHs, ThreeCHs);
        HashTable ThreeGram02[5000];
        int p02;
        p02 = AddHashGram(ThreeGram02, ThreeCHs);
        return count_hashTable(OneGram01, n01, OneGram02, n02) + count_hashTable(TwoGram01, m01, TwoGram02, m02) + count_hashTable(ThreeGram01, p01, ThreeGram02, p02);
    }
    return 0;
}

int ListFile_hashTable(List A[], string link, string type)
{
    string folder = link;
    vector<string> files = vector<string>();
    Select_folder(folder, files);
    int n = 0;
    for(int i = 0; i < files.size(); i++)
    {
        for (int j = i+1 ; j < files.size(); j++)
        {
            A[n].KQ = compare_hashTable_grams(files[i], files[j], type);
            A[n].file01 = files[i];
            A[n].file02 = files[j];
            n++;
        }
    }
    return n;
}


int main(int argc, char* argv[])
{
    string link = argv[2];
    string CTDL = argv[4];
    string Type = argv[6];
    int x = atoi(argv[8]);

    auto start = high_resolution_clock::now();
    List A[1000];
    int n;
    if (CTDL == "vector")
    {
        n = ListFile_vector(A, link, Type);
        QuickSort(A, 0, n);
        for (int i = 0; i < x; i++)
        {
            A[i].Xuat();
        }
    }
    else if (CTDL == "bst")
    {
        n = ListFile_bst(A, link, Type);
        QuickSort(A, 0, n);
        for (int i = 0; i < x; i++)
        {
            A[i].Xuat();
        }
    }
    else if (CTDL == "map")
    {
        n = ListFile_map(A, link, Type);
        QuickSort(A, 0, n);
        for (int i = 0; i < x; i++)
        {
            A[i].Xuat();
        }
    }
    else if (CTDL == "map_from_scratch")
    {
        n = ListFile_hashTable(A, link, Type);
        QuickSort(A, 0, n);
        for (int i = 0; i < x; i++)
        {
            A[i].Xuat();
        }
    }

    auto finish = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(finish - start);
    cout<<"Running time: "<<duration.count()/1000000.0 << " seconds" << endl;

    system("pause");
    return 0;
}
