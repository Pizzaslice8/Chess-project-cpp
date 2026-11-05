#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>

#define SPACE_SIZE 0x800000
#define EMPTY_SEED 0xF507F230450691BU
#define ZOBRIST_WCS 0x7A44023D058C0AFU
#define ZOBRIST_WCL 0x1548865A66A407C4U
#define ZOBRIST_BCS 0xF397C2CD73E605DU
#define ZOBRIST_BCL 0xB9697CCBCD8311EU
#define ZOBRIST_WHITE_TO_MOVE 0xC2A427E56F8ED21U
#define ZOBRIST_BLACK_TO_MOVE 0x10115F7180C695A8U

const int knightBonus[] = {
    -12, -10, -8, -8, -8, -8, -10, -12, -10, -10, -8, -5, -5, -8, -10, -10,
    -5, 0, 5, 5, 5, 5, 0, -5, -2, 0, 5, 5, 5, 5, 0, -2, 0, 2, 8, 10, 10, 10, 2, 0,
    -5, 0, 5, 8, 8, 5, 0, -5, -8, -8, -5, -5, -5, -5, -8, -8, -12, -5, -5, -5,
    -5, -5, -5, -12
};
const int bishopDx[] = {1, 1, -1, -1};
const int bishopDy[] = {-1, 1, 1, -1};
const int rookDx[] = {1, 0, -1, 0};
const int rookDy[] = {0, 1, 0, -1};
const unsigned long long whitePawnVision[] = {
0x200U, 0x500U, 0xa00U, 0x1400U, 0x2800U, 0x5000U, 0xa000U, 0x4000U, 0x20000U, 0x50000U, 0xa0000U, 0x140000U,
0x280000U, 0x500000U, 0xa00000U, 0x400000U, 0x2000000U, 0x5000000U, 0xa000000U, 0x14000000U, 0x28000000U,
0x50000000U, 0xa0000000U, 0x40000000U, 0x200000000U, 0x500000000U, 0xa00000000U, 0x1400000000U, 0x2800000000U,
0x5000000000U, 0xa000000000U, 0x4000000000U, 0x20000000000U, 0x50000000000U, 0xa0000000000U, 0x140000000000U,
0x280000000000U, 0x500000000000U, 0xa00000000000U, 0x400000000000U, 0x2000000000000U, 0x5000000000000U,
0xa000000000000U, 0x14000000000000U, 0x28000000000000U, 0x50000000000000U, 0xa0000000000000U, 0x40000000000000U,
0x200000000000000U, 0x500000000000000U, 0xa00000000000000U, 0x1400000000000000U, 0x2800000000000000U,
0x5000000000000000U, 0xa000000000000000U, 0x4000000000000000U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
};
const unsigned long long blackPawnVision[] = {
0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0x2U, 0x5U, 0xaU, 0x14U, 0x28U, 0x50U, 0xa0U, 0x40U, 0x200U, 0x500U,
0xa00U, 0x1400U, 0x2800U, 0x5000U, 0xa000U, 0x4000U, 0x20000U, 0x50000U, 0xa0000U, 0x140000U, 0x280000U,
0x500000U, 0xa00000U, 0x400000U, 0x2000000U, 0x5000000U, 0xa000000U, 0x14000000U, 0x28000000U, 0x50000000U,
0xa0000000U, 0x40000000U, 0x200000000U, 0x500000000U, 0xa00000000U, 0x1400000000U, 0x2800000000U, 0x5000000000U,
0xa000000000U, 0x4000000000U, 0x20000000000U, 0x50000000000U, 0xa0000000000U, 0x140000000000U, 0x280000000000U,
0x500000000000U, 0xa00000000000U, 0x400000000000U, 0x2000000000000U, 0x5000000000000U, 0xa000000000000U,
0x14000000000000U, 0x28000000000000U, 0x50000000000000U, 0xa0000000000000U, 0x40000000000000U
};
const unsigned long long kingVision[] = {
0x302U, 0x705U, 0xe0aU, 0x1c14U, 0x3828U, 0x7050U, 0xe0a0U, 0xc040U, 0x30203U, 0x70507U, 0xe0a0eU, 0x1c141cU,
0x382838U, 0x705070U, 0xe0a0e0U, 0xc040c0U, 0x3020300U, 0x7050700U, 0xe0a0e00U, 0x1c141c00U, 0x38283800U,
0x70507000U, 0xe0a0e000U, 0xc040c000U, 0x302030000U, 0x705070000U, 0xe0a0e0000U, 0x1c141c0000U, 0x3828380000U,
0x7050700000U, 0xe0a0e00000U, 0xc040c00000U, 0x30203000000U, 0x70507000000U, 0xe0a0e000000U, 0x1c141c000000U,
0x382838000000U, 0x705070000000U, 0xe0a0e0000000U, 0xc040c0000000U, 0x3020300000000U, 0x7050700000000U,
0xe0a0e00000000U, 0x1c141c00000000U, 0x38283800000000U, 0x70507000000000U, 0xe0a0e000000000U, 0xc040c000000000U,
0x302030000000000U, 0x705070000000000U, 0xe0a0e0000000000U, 0x1c141c0000000000U, 0x3828380000000000U,
0x7050700000000000U, 0xe0a0e00000000000U, 0xc040c00000000000U, 0x203000000000000U, 0x507000000000000U,
0xa0e000000000000U, 0x141c000000000000U, 0x2838000000000000U, 0x5070000000000000U, 0xa0e0000000000000U,
0x40c0000000000000U
};
const unsigned long long knightVision[] = {
0x20400U, 0x50800U, 0xa1100U, 0x142200U, 0x284400U, 0x508800U, 0xa01000U, 0x402000U, 0x2040004U, 0x5080008U,
0xa110011U, 0x14220022U, 0x28440044U, 0x50880088U, 0xa0100010U, 0x40200020U, 0x204000402U, 0x508000805U,
0xa1100110aU, 0x1422002214U, 0x2844004428U, 0x5088008850U, 0xa0100010a0U, 0x4020002040U, 0x20400040200U,
0x50800080500U, 0xa1100110a00U, 0x142200221400U, 0x284400442800U, 0x508800885000U, 0xa0100010a000U, 0x402000204000U,
0x2040004020000U, 0x5080008050000U, 0xa1100110a0000U, 0x14220022140000U, 0x28440044280000U, 0x50880088500000U,
0xa0100010a00000U, 0x40200020400000U, 0x204000402000000U, 0x508000805000000U, 0xa1100110a000000U, 0x1422002214000000U,
0x2844004428000000U, 0x5088008850000000U, 0xa0100010a0000000U, 0x4020002040000000U, 0x400040200000000U,
0x800080500000000U, 0x1100110a00000000U, 0x2200221400000000U, 0x4400442800000000U, 0x8800885000000000U,
0x100010a000000000U, 0x2000204000000000U, 0x4020000000000U, 0x8050000000000U, 0x110a0000000000U, 0x22140000000000U,
0x44280000000000U, 0x88500000000000U, 0x10a00000000000U, 0x20400000000000U
};

unsigned long long zobrist[64][13];

int limit, maxDepth, slot = 0, undoDeep = 0, historyStored = 0;
int posData[7];
unsigned long long pieceBoard[12];
unsigned long long currentZobrist;
unsigned long long undoStackPieces[50][12];
int undoStackData[50][7];
int killerMoves[50];
int history[500][10];

typedef struct {
    char string[3];
} StringSizeTwo;

int main() {
    printf("Initializing... ");
    initialize();
    printf("done\n");
    Sleep(500);
    printf("FEN: ");
    char fen[100];
    fgets(fen, sizeof(fen), stdin);
    printf("\nDepth (number of full moves): ");
    scanf("%d", maxDepth);
    maxDepth *= 2;
    printf("\nTime limit: ");
    scanf("%d", limit);
    return 0;
}
void initialize(void) {
    srand(time(NULL));
}
unsigned long long largeRand(void) {
    return (rand() << 48) | (rand() << 32) | (rand() << 16) | rand(); 
}
int parseSquare(char alpha, char numeric) {
    return (numeric - 49) * 8 + (alpha - 97);
}
StringSizeTwo parseSquareId(int id) {
    StringSizeTwo output;
    output.string[0] = (getX(id) + 97);
    output.string[1] = (getY(id) + 49);
    output.string[2] = '\0';
    return output;
}
static inline int getX(int square) {
    return (square % 8);
}
static inline int getY(int square) {
    return (square / 8);
}
int translate(int square, int x, int y) {
    int squareX = getX(square);
    int squareY = getY(square);
    squareX += x;
    squareY += y;
    if (squareX < 0 || squareX >= 8 || squareY < 0 || squareY >= 8) return -1;
    return 8 * y + x;
}
int getId(char piece) {
    switch (piece) {
        case 'K': return 0;
        case 'k': return 1;
        case 'Q': return 2;
        case 'q': return 3;
        case 'R': return 4;
        case 'r': return 5;
        case 'B': return 6;
        case 'b': return 7;
        case 'N': return 8;
        case 'n': return 9;
        case 'P': return 10;
        case 'p': return 11;
        default: return 12;
    }
}
char getDisplay(int pieceid) {
    switch (pieceid) {
        case 0: return 'K';
        case 1: return 'k';
        case 2: return 'Q';
        case 3: return 'q';
        case 4: return 'R';
        case 5: return 'r';
        case 6: return 'B';
        case 7: return 'b';
        case 8: return 'N';
        case 9: return 'n';
        case 10: return 'P';
        case 11: return 'p';
        default: return '\0';
    }
}
static inline int color(int piece) {
    return ((piece % 2) == 0);
}
int getPiece(int square) {
    if (square == 0) return -1;
    for (int i = 0; i < 12; i++) {
        if ((pieceBoard[i] & (1U < square)) != 0) return i;
    }
    return -1;
}
int countNonzeros(int inputArray[]) {
    int count = 0;
    for (int element : inputArray) {
        if (element != 0) count++;
    }
    return count;
}
int closestEdge(int square) {
    int output = 1;
    //...
    return output;
}
unsigned long long getZobrist(unsigned long long boards[], int extraData[]) {
    unsigned long long out = EMPTY_SEED;
    for (int i = 0; i < 12; i++) {
        //...
    }
    out ^= ((extraData[0] == 1) ? ZOBRIST_WHITE_TO_MOVE : ZOBRIST_BLACK_TO_MOVE);
    if (extraData[1] == 1) out ^= ZOBRIST_WCS;
    if (extraData[2] == 1) out ^= ZOBRIST_WCL;
    if (extraData[3] == 1) out ^= ZOBRIST_BCS;
    if (extraData[4] == 1) out ^= ZOBRIST_BCL;
    if (extraData[5] != 0) out ^= zobrist[extraData[5]][12];
    return out;
}

