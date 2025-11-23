/**************************
 Exercise:  CPP2C
 Date:     12/11/25
 Developer: Baruch Haimson
 Reviewer:  Maria
 Status:
***************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc, free */

/* simple template macros */
#define TEMPLATE_MAX(T) \
    T Max_##T(T a, T b) \
    {                   \
        return a > b ? a : b; \
    }

TEMPLATE_MAX(int)
TEMPLATE_MAX(double)

static int s_count = 0;

/* ========= Forward declarations ================= */
typedef struct PT PT;
typedef struct Minibus Minibus;
typedef struct ArmyMinibus ArmyMinibus;
typedef struct Taxi Taxi;
typedef struct SpecialTaxi SpecialTaxi;
typedef struct PublicConvoy PublicConvoy;

/* function-pointer typedefs */
typedef void (*PTFunc)(PT*);
typedef void (*MBWashFunc)(Minibus*, int);

/* ========= vtable structures =================== */
typedef struct Vtable
{
    PTFunc dtor;
    PTFunc display;
} Vtable;

typedef struct MinibusVtable
{
    Vtable base;
    MBWashFunc wash;
} MinibusVtable;

/* ========= entity structures ================== */
struct PT
{
    const Vtable* vptr;
    int m_license_plate;
};

struct Minibus
{
    PT pt;
    int m_numSeats;
};

struct ArmyMinibus
{
    Minibus minibus;
};

struct Taxi
{
    PT pt;
};

struct SpecialTaxi
{
    Taxi taxi;
};

struct PublicConvoy
{
    PT pt;
    PT* m_pt1;
    PT* m_pt2;
    Minibus m_m;
    Taxi m_t;
};

/* ========= function declarations ============== */
/* PT */
void PTctor(PT* this);
void PTcctor(PT* this, const PT* other);
void PTdtor(PT* this);
void PTdisplay(PT* this);
int  PTGetID(PT* this);
void PTPrintCount(void);

/* Minibus */
void MinibusCtor(Minibus* this);
void MinibusCctor(Minibus* this, const Minibus* other);
void Minibusdtor(Minibus* this);
void Minibusdisplay(Minibus* this);
void MinibusWash(Minibus* this, int minutes);

/* ArmyMinibus */
void ArmyMinibusCtor(ArmyMinibus* this);
void ArmyMinibusdtor(ArmyMinibus* this);

/* Taxi */
void TaxiCtor(Taxi* this);
void TaxiCctor(Taxi* this, const Taxi* other);
void Taxidtor(Taxi* this);
void Taxidisplay(Taxi* this);

/* SpecialTaxi */
void SpecialTaxiCtor(SpecialTaxi* this);
void SpecialTaxidtor(SpecialTaxi* this);
void SpecialTaxidisplay(SpecialTaxi* st);

/* PublicConvoy */
void PublicConvoyCtor(PublicConvoy* this);
void PublicConvoyCctor(PublicConvoy* this, const PublicConvoy* other);
void PublicConvoyDtor(PublicConvoy* this);
void PublicConvoyDisplay(PublicConvoy* this);

/* helpers / print wrappers */
void print_info_PT(PT* p);
void print_info_void(void);
void print_info_Minibus(Minibus* m);
void print_info_int(int i, PT* this);
void taxi_display(Taxi* t);

/* ========= vtables ============================ */

/* forward declare vtables (definitions below) */
static const Vtable PT_vtable;
static const Vtable Taxi_vtable;
static const Vtable SpecialTaxi_vtable;
static const MinibusVtable Minibus_vtable;
static const MinibusVtable ArmyMinibus_vtable;
static const Vtable PublicConvoy_vtable;

/* ===== PT implementations ===== */
void PTctor(PT* this)
{
    this->vptr = &PT_vtable;
    this->m_license_plate = ++s_count;
    printf("PublicTransport::Ctor()%d\n", this->m_license_plate);
}

void PTcctor(PT* this, const PT* other)
{
    (void)other;
    this->vptr = &PT_vtable;
    this->m_license_plate = ++s_count;
    printf("PublicTransport::CCtor() %d\n", this->m_license_plate);
}

void PTdtor(PT* this)
{
    printf("PublicTransport::Dtor()%d\n", this->m_license_plate);
    --s_count;
}

void PTdisplay(PT* this)
{
    printf("PublicTransport::display(): %d\n", this->m_license_plate);
}

int PTGetID(PT* this)
{
    return this->m_license_plate;
}

void PTPrintCount(void)
{
    printf("s_count: %d\n", s_count);
}

/* ===== Minibus implementations ===== */
void MinibusCtor(Minibus* this)
{
    PTctor(&this->pt);
    ((PT*)this)->vptr = (const Vtable*)&Minibus_vtable;
    this->m_numSeats = 20;
    printf("Minibus::Ctor()\n");
}

void MinibusCctor(Minibus* this, const Minibus* other)
{
    PTcctor(&this->pt, &other->pt);
    ((PT*)this)->vptr = (const Vtable*)&Minibus_vtable;
    this->m_numSeats = other->m_numSeats;
    printf("Minibus::CCtor()\n");
}

void Minibusdtor(Minibus* this)
{
    printf("Minibus::Dtor()\n");
    ((PT*)this)->vptr = &PT_vtable;
    PTdtor(&this->pt);
}

void Minibusdisplay(Minibus* this)
{
    printf("Minibus::display() ID:%d num seats:%d\n",
           PTGetID((PT*)this), this->m_numSeats);
}

void MinibusWash(Minibus* this, int minutes)
{
    printf("Minibus::wash(%d) ID:%d\n", minutes, PTGetID((PT*)this));
}

/* ===== ArmyMinibus ===== */
void ArmyMinibusCtor(ArmyMinibus* this)
{
    MinibusCtor(&this->minibus);
    ((PT*)&this->minibus)->vptr = (const Vtable*)&ArmyMinibus_vtable;
    printf("ArmyMinibus::Ctor()\n");
}

void ArmyMinibusdtor(ArmyMinibus* this)
{
    printf("ArmyMinibus::Dtor()\n");
    ((PT*)&this->minibus)->vptr = (const Vtable*)&Minibus_vtable;
    Minibusdtor(&this->minibus);
}

/* ===== Taxi ===== */
void TaxiCtor(Taxi* this)
{
    PTctor(&this->pt);
    ((PT*)this)->vptr = (const Vtable*)&Taxi_vtable;
    printf("Taxi::Ctor()\n");
}

void TaxiCctor(Taxi* this, const Taxi* other)
{
    PTcctor(&this->pt, &other->pt);
    ((PT*)this)->vptr = (const Vtable*)&Taxi_vtable;
    printf("Taxi::CCtor()\n");
}

void Taxidtor(Taxi* this)
{
    printf("Taxi::Dtor()\n");
    ((PT*)this)->vptr = &PT_vtable;
    PTdtor(&this->pt);
}

void Taxidisplay(Taxi* this)
{
    printf("Taxi::display() ID:%d\n", PTGetID((PT*)this));
}

/* ===== SpecialTaxi ===== */
void SpecialTaxiCtor(SpecialTaxi* this)
{
    TaxiCtor(&this->taxi);
    ((PT*)&this->taxi)->vptr = (const Vtable*)&SpecialTaxi_vtable;
    printf("SpecialTaxi::Ctor()\n");
}

void SpecialTaxidtor(SpecialTaxi* this)
{
    printf("SpecialTaxi::Dtor()\n");
    ((PT*)&this->taxi)->vptr = &Taxi_vtable;
    Taxidtor(&this->taxi);
}

void SpecialTaxidisplay(SpecialTaxi* st)
{
    printf("SpecialTaxi::display() ID:%d\n", PTGetID((PT*)st));
}

/* ===== PublicConvoy implementations ===== */

void PublicConvoyCtor(PublicConvoy* this)
{
    PTctor(&this->pt);
    this->pt.vptr = &PublicConvoy_vtable;
    this->m_pt1 = (PT*)malloc(sizeof(Minibus));
    MinibusCtor((Minibus*)this->m_pt1);
    ((PT*)this->m_pt1)->vptr = (const Vtable*)&Minibus_vtable;

    this->m_pt2 = (PT*)malloc(sizeof(Taxi));
    TaxiCtor((Taxi*)this->m_pt2);
    ((PT*)this->m_pt2)->vptr = &Taxi_vtable;

    MinibusCtor(&this->m_m);
    TaxiCtor(&this->m_t);

}


void PublicConvoyCctor(PublicConvoy* this, const PublicConvoy* other)
{
    PTcctor(&this->pt, &other->pt);
    this->pt.vptr = &PublicConvoy_vtable;

    if (other->m_pt1) 
    {
        if (other->m_pt1->vptr == (Vtable*)&Minibus_vtable) 
        {
            this->m_pt1 = (PT*)malloc(sizeof(Minibus));
            MinibusCctor((Minibus*)this->m_pt1, (Minibus*)other->m_pt1);
        } 
        else if (other->m_pt1->vptr == (Vtable*)&Taxi_vtable) 
        {
            this->m_pt1 = (PT*)malloc(sizeof(Taxi));
            TaxiCctor((Taxi*)this->m_pt1, (Taxi*)other->m_pt1);
        }
    } 
    else 
    {
        this->m_pt1 = NULL;
    }

    if (other->m_pt2) 
    {
        if (other->m_pt2->vptr == (Vtable*)&Minibus_vtable) 
        {
            this->m_pt2 = (PT*)malloc(sizeof(Minibus));
            MinibusCctor((Minibus*)this->m_pt2, (Minibus*)other->m_pt2);
        } 
        else if (other->m_pt2->vptr == (Vtable*)&Taxi_vtable) 
        {
            this->m_pt2 = (PT*)malloc(sizeof(Taxi));
            TaxiCctor((Taxi*)this->m_pt2, (Taxi*)other->m_pt2);
        }
    } 
    else 
    {
        this->m_pt2 = NULL;
    }

    MinibusCctor(&this->m_m, &other->m_m);
    TaxiCctor(&this->m_t, &other->m_t);

    printf("PublicConvoy::CCtor()\n");
}


void PublicConvoyDtor(PublicConvoy* this)
{
    if (this->m_pt1) 
    {
        this->m_pt1->vptr->dtor(this->m_pt1);
        free(this->m_pt1);
        this->m_pt1 = NULL;
    }

    if (this->m_pt2) 
    {
        this->m_pt2->vptr->dtor(this->m_pt2);
        free(this->m_pt2);
        this->m_pt2 = NULL;
    }

    Taxidtor(&this->m_t);
    Minibusdtor(&this->m_m);

    ((PT*)&this->pt)->vptr = &PT_vtable;
    PTdtor(&this->pt);
}

void PublicConvoyDisplay(PublicConvoy* this)
{
    if (this->m_pt1) this->m_pt1->vptr->display(this->m_pt1);
    if (this->m_pt2) this->m_pt2->vptr->display(this->m_pt2);
    Minibusdisplay(&this->m_m);
    Taxidisplay(&this->m_t);
}

/* ===== vtable definitions (now with clone pointers) ===== */

static const Vtable PT_vtable = { (PTFunc)PTdtor, (PTFunc)PTdisplay };
static const Vtable Taxi_vtable = { (PTFunc)Taxidtor, (PTFunc)Taxidisplay };
static const Vtable SpecialTaxi_vtable = { (PTFunc)SpecialTaxidtor, (PTFunc)SpecialTaxidisplay };
static const MinibusVtable Minibus_vtable = { { (PTFunc)Minibusdtor, (PTFunc)Minibusdisplay }, (MBWashFunc)MinibusWash };
static const MinibusVtable ArmyMinibus_vtable = { { (PTFunc)ArmyMinibusdtor, (PTFunc)Minibusdisplay }, (MBWashFunc)MinibusWash };
static const Vtable PublicConvoy_vtable = { (PTFunc)PublicConvoyDtor, (PTFunc)PublicConvoyDisplay };

/* ===== helpers / print wrappers ===== */

void print_info_PT(PT* p)
{
    p->vptr->display(p);
}

void print_info_void(void)
{
    PTPrintCount();
}

void print_info_Minibus(Minibus* m)
{
    ((MinibusVtable*)m->pt.vptr)->wash(m,3);
}

void print_info_int(int i, PT* this)
{
    Minibus ret = {0};

    (void)i;
    MinibusCtor(&ret);
    printf("print_info(int i)\n");
    Minibusdisplay(&ret);

    PTcctor(this, &ret.pt);

    Minibusdtor(&ret);
}

void taxi_display(Taxi* src)
{
    Taxidisplay(src);
}

/* ===== main ===== */

int main(void)
{
    int i = 0;
    PT ret_val = {0};
    Minibus m = {0};
    PT* array[3] = {0};
    PT arr2[3] = {0};
    Minibus m2 = {0};
    Minibus arr3[4] = {0};
    Taxi* arr4 = NULL;
    SpecialTaxi st = {0};
    ArmyMinibus* army_minibus = NULL;
    PublicConvoy* pc1 = NULL;
    PublicConvoy* pc2 = NULL;
    Minibus tmpM = {0};
    Taxi tmpT = {0};
    Taxi tmp = {0};

    /* === Minibus demo === */
    MinibusCtor(&m);
    print_info_Minibus(&m);

    /* === print_info(int) === */
    print_info_int(3, &ret_val);
    PTdisplay(&ret_val);
    PTdtor(&ret_val);

    /* === array of PT* (new Minibus, new Taxi, new Minibus) === */
    array[0] = (PT*)malloc(sizeof(Minibus));
    MinibusCtor((Minibus*)array[0]);

    array[1] = (PT*)malloc(sizeof(Taxi));
    TaxiCtor((Taxi*)array[1]);

    
    array[2] = (PT*)malloc(sizeof(Minibus));
    MinibusCtor((Minibus*)array[2]);

    for (i = 0; i < 3; ++i) {
        array[i]->vptr->display(array[i]);
    }

    for (i = 0; i < 3; ++i) {
        array[i]->vptr->dtor(array[i]);
        free(array[i]);
    }

    /* === arr2: slicing simulation === */
    MinibusCtor(&tmpM);
    PTcctor(&arr2[0], &tmpM.pt);

    TaxiCtor(&tmpT);
    PTcctor(&arr2[1], &tmpT.pt);

    PTctor(&arr2[2]);

    tmpT.pt.vptr->dtor(&tmpT.pt);
    tmpM.pt.vptr->dtor(&tmpM.pt);

    for (i = 0; i < 3; ++i) {
        arr2[i].vptr->display(&arr2[i]);
    }

    print_info_PT(&arr2[0]);

    PTPrintCount();
    MinibusCtor(&m2);
    PTPrintCount();

    /* === arr3 and arr4 === */
    for (i = 0; i < 4; ++i) {
        MinibusCtor(&arr3[i]);
    }

    arr4 = (Taxi*)malloc(4 * sizeof(Taxi));
    for (i = 0; i < 4; ++i) {
        TaxiCtor(&arr4[i]);
    }

    for (i = 3; i >= 0; --i) {
        arr4[i].pt.vptr->dtor(&arr4[i].pt);
    }
    free(arr4);

    printf("%d\n", Max_int(1,2));
    printf("%d\n", Max_int(1,2.0f));

    /* === SpecialTaxi === */
    SpecialTaxiCtor(&st);
    TaxiCctor(&tmp, &st.taxi);
    taxi_display(&tmp);
    Taxidtor(&tmp);

    /* === PublicConvoy deep-copy demo === */

    pc1 = (PublicConvoy*)malloc(sizeof(PublicConvoy));
    PublicConvoyCtor(pc1);

    pc2 = (PublicConvoy*)malloc(sizeof(PublicConvoy));
    PublicConvoyCctor(pc2, pc1);

    PublicConvoyDisplay(pc1);
    PublicConvoyDisplay(pc2);

    PublicConvoyDtor(pc1);
    free(pc1);

    PublicConvoyDisplay(pc2);
    PublicConvoyDtor(pc2);
    free(pc2);

    /* === ArmyMinibus === */
    army_minibus = (ArmyMinibus*)malloc(sizeof(ArmyMinibus));
    ArmyMinibusCtor(army_minibus);
    army_minibus->minibus.pt.vptr->display(&army_minibus->minibus.pt);
    ((MinibusVtable*)army_minibus->minibus.pt.vptr)->wash(&army_minibus->minibus, 5);
    army_minibus->minibus.pt.vptr->dtor(&army_minibus->minibus.pt);
    free(army_minibus);

    /* === cleanup === */
    SpecialTaxidtor(&st);

    for (i = 3; i >= 0; --i) {
        arr3[i].pt.vptr->dtor(&arr3[i].pt);
    }

    m2.pt.vptr->dtor(&m2.pt);

    for (i = 2; i >= 0; --i) {
        arr2[i].vptr->dtor(&arr2[i]);
    }

    m.pt.vptr->dtor(&m.pt);

    return 0;
}
