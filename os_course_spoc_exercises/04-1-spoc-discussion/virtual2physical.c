#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

// input file
int page[128][32], disk[128][32];

int char_to_int(char a) {
	int b;
	if (a < 'a') {
		b = a - '0';
	}
	else {
		b = a - 'a' + 10;
	}
	return b;
}

// 读入page.in, disk.in
void input(char* filename, int flag) {
	ifstream in(filename);
	if (! in.is_open()) {
		cout << "Error: cannot open " << filename << endl;
		return -1;
	}
	char buf[110];
	int row = 128, col = 32;
	while(row--) {
		in.getline(buf, 110);
		cout << buf << endl;
		for (int i = 0; i < col; i++) {
			if (flag) {
				disk[127-row][i] = 16 * char_to_int(buf[i * 3 + 9]) + char_to_int(buf[i * 3 + 10]);
			}
			else {
				page[127-row][i] = 16 * char_to_int(buf[i * 3 + 9]) + char_to_int(buf[i * 3 + 10]);
			}
		}
	}
}

// PDBR content: 0xd80（1101_100 0_0000, page 0x6c）
int PDBR = 0x6c;

int calc(int addr){
	printf("Virtual Address %x:)\n", addr);
	//change addr to 3 offset：pde/pte/physical offset
	//& 31(11111)：取5位二进制
	int pde_off = (addr >> 10) & 31;
	int pte_off = (addr >> 5) & 31;
	int phy_off = addr & 31;
	//find page directory entry(pde)的内容(content)
	int pde_con = page[PDBR][pde_off];
	//is valid ?
	bool pde_is_valid = pde_con >> 7;
	// 127：1111111(7-bit)
	printf("  --> pde index:0x%x  pde contents:(valid %d, pfn 0x%x)\n", pde_off, pde_is_valid, pde_con & 127);
	//pde index:pde_off, pde contents:(pde_con, binary: valid:pde_is_valid, pfn:pde_con&127)
	if (pde_is_valid){
		//去掉valid位，得到对应的数组地址
		int pte_addr = pde_con & 127;
		//通过pte offset 来找pte content
		int pte_con = page[pte_addr][pte_off];
		//is valid ?
		bool pte_is_valid = pte_con >> 7;
		printf("  --> pte index:0x%x  pte contents:(valid %d, pfn 0x%x)\n", pte_off, pte_is_valid, pte_con & 127);
		//pte index:pte_off, pte contents:(pte_con, binary: valid:pte_is_valid, pfn:pte_con&127
		if (pte_is_valid){
			//去掉valid位，得到对应的数组地址
			int phy_addr = pte_con & 127;
			//通过phy offset 来找phy content
			int phy_con = page[phy_addr][phy_off];
			printf("      --> To Physical Address 0x%x --> value: %x\n", (32 * (pte_con & 127) + phy_off), phy_con);
			return phy_con;
		} else {
			//去掉valid位，得到对应的数组地址
			int disk_addr = pte_con & 127;
			//通过phy offset 来找disk content
			int disk_con = disk[disk_addr][phy_off];
			printf("      --> To Disk Sector Address 0x%x --> value : 0x%x\n", (32 * (pte_con & 127) + phy_off), disk_con);
			return disk_con;
		}
	} else {
		printf("    --> Fault (page directory entry not valid)\n");
		return -1;
	}
}

int main(){
	input("page.in", 0);
	input("disk.in", 1);
	for(int i = 0; i < 32; i++)
		cout << page[127][i] << ' ';
	calc(0x6653);
	calc(0x1c13);
	calc(0x6890);
	calc(0xaf6);
	calc(0x1e6f);
	return 0;
}