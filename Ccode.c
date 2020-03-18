#include"Main.h"
int main(){
int round=0;
int x;
int Starting_index;
int hop_count;
int index = 0;
for(round = 0; round < 1; round++) {
Starting_index = gPasswordHash[0] * 256 + gPasswordHash[1];
hop_count = gPasswordHash[2+round*4] * 256 + gPasswordHash[3+round*4];
if(hop_count == 0) hop_count = 0xFFFF;
index = Starting_index;
for ( x = 0; x < fileLength(gInFileName); x++) { // Note: length passed in
gOutFileName[x] = gOutFileName[x] ^ gkey[index];
index += hop_count;
if(index >= 65537) index -= 65537;
}
}
}