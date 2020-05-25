
#include <iostream>
#include <fstream>



using namespace std;

ifstream f ("abcd.in");
ofstream g ("abcd.out");


class nod
{
    int *valori;
    int t;	 // grad maxim
    nod **C; // vector de pointeri
    int n;	 // nr curent de valori
    bool eFrunza; // true daca este frunza
public:
    nod(int _t, bool _eFrunza);


    void inserareVal(int k);


    void split(int i, nod *y);


    void printare();

    void minim4(int k);
    void maxim5(int k);
    void interval6(int val1,int val2);


    nod *search(int k);


    friend class BTree;
};


class BTree
{
    nod *root; //Pointer la radacina
    int t; //Grad maxim
public:

    BTree(int _t)
    {
        root = NULL;
        t = _t;
    }


    void printare()
    {
        if (root != NULL) root->printare();
    }
    void interval6(int val1,int val2)
    {
        if (root != NULL) root->interval6(val1,val2);
    }

    void minim4(int k)
    {
        if (root != NULL) root->minim4(k);

    }
    void maxim5(int k)
    {
        if (root != NULL) root->maxim5(k);

    }


    nod* search(int k)
    {
        return (root == NULL)? NULL : root->search(k);
    }


    void insert(int k);
};


nod::nod(int t1, bool eFrunza1)
{

    t = t1;
    eFrunza = eFrunza1;

    //alocare memorie
    valori = new int[2*t-1];
    C = new nod *[2*t];


    n = 0;
}


void nod::printare()
{

    int i;
    for (i = 0; i < n; i++)
    {

        if (eFrunza == false)
            C[i]->printare();
        cout << " " << valori[i];
    }

    // printare ultim set de valori
    if (eFrunza == false)
        C[i]->printare();
    cout<<"\n";
}

void nod::minim4(int k)
{

    int i,ok=0;
    int minim=9999999;
    for (i=0 ; i < n; i++)
    {

        if (eFrunza == false)
            C[i]->minim4(k);
        if (valori[i]<minim&&valori[i]>=k)
        {
            minim=valori[i];
            ok=1;
        }

    }


    if (eFrunza == false)
        C[i]->minim4(k);
    if (ok==0) minim=-1;

    cout<< minim<<"\n";
}

void nod::maxim5(int k)
{

    int i,ok=0;
    int maxim=-1;
    for (i=0 ; i < n; i++)
    {

        if (eFrunza == false)
            C[i]->maxim5(k);
        if (valori[i]>maxim&&valori[i]<=k)
        {
            maxim=valori[i];
            ok=1;
        }

    }


    if (eFrunza == false)
        C[i]->maxim5(k);
    if (ok==0) maxim=-1;

    cout<< maxim<<"\n";
}
void nod::interval6(int val1,int val2)
{

    int i,ok=0;
    for (i = 0; i < n; i++)
    {

        if (eFrunza == false)
            C[i]->interval6(val1,val2);
        if (val1<=valori[i]&&valori[i]<=val2)
            cout <<valori[i]<<" ";
        ok=1;
    }


    if (eFrunza == false)
        C[i]->interval6(val1,val2);
    if (ok==0) cout<<"-1";
    cout<<"\n";
}




nod *nod::search(int k)
{

    int i = 0;
    while (i < n && k > valori[i])
        i++;


    if (valori[i] == k)
        return this;


    if (eFrunza == true)
        return NULL;


    return C[i]->search(k);
}


void BTree::insert(int k)
{
    // verif daca arborele este gol
    if (root == NULL)
    {
        // alocare memorie
        root = new nod(t, true);
        root->valori[0] = k; // inserare
        root->n = 1; // update nr val
    }
    else
    {
        // daca radacina este plina, se creeaza un nou nivel
        if (root->n == 2*t-1)
        {
            // alocare memorie pt radacina
            nod *s = new nod(t, false);


            s->C[0] = root;

            // impartire radacina
            s->split(0, root);
            int i = 0;
            if (s->valori[0] < k)
                i++;
            s->C[i]->inserareVal(k);

            // schimbare radacina
            root = s;
        }
        else
            root->inserareVal(k);
    }
}


void nod::inserareVal(int k)
{

    int i = n-1;

    //daca nod este frunza
    if (eFrunza == true)
    {

        while (i >= 0 && valori[i] > k)
        {
            valori[i+1] = valori[i];
            i--;
        }


        valori[i+1] = k;
        n = n+1;
    }
    else // daca nod nu este frunza
    {

        while (i >= 0 && valori[i] > k)
            i--;


        if (C[i+1]->n == 2*t-1)
        {

            split(i+1, C[i+1]);


            if (valori[i+1] < k)
                i++;
        }
        C[i+1]->inserareVal(k);
    }
}

void nod::split(int i, nod *y)
{
    //creare nod nou
    nod *z = new nod(y->t, y->eFrunza);
    z->n = t - 1;

    // copiere valori
    for (int j = 0; j < t-1; j++)
        z->valori[j] = y->valori[j+t];
    if (y->eFrunza == false)
    {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j+t];
    }

    y->n = t - 1;


    for (int j = n; j >= i+1; j--)
        C[j+1] = C[j];
    C[i+1] = z;

    for (int j = n-1; j >= i; j--)
        valori[j+1] = valori[j];

    valori[i] = y->valori[t-1];
    n = n + 1;
}


int main()
{
    int gradMax,nrOp,val,op,val1,val2;

    f>>gradMax;

    BTree t(gradMax);

    f>>nrOp;
    for(int i=1; i<=nrOp; i++)
    {
        f>>op;
        switch(op)
        {
        case 1:
            f>>val;
            t.insert(val);
            t.printare();
            break;
        case 3:
            f>>val;
            (t.search(val) != NULL)? cout<<"1 \n":cout<<"0 \n";
            break;
        case 4:
            f>>val;
            t.minim4(val);
            break;
        case 5:
            f>>val;
            t.maxim5(val);
            break;
        case 6:
            f>>val1>>val2;
            t.interval6(val1,val2);
            break;

        }
    }

    return 0;
}
