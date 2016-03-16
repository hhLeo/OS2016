#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstdlib>
#include<string.h>
#include<math.h>

using namespace std;

const int pageSize = 32;//页大小，32B

int HextoDec(char *hex){//十六进制转化为10进制
    int i;
    int length = strlen(hex);
    int num = 0;
    for(i = 0; i < length; i++){
    	if(hex[i] >= '0' && hex[i] <= '9'){
    		num = num + (hex[i] - '0') * int(pow(16, length - i - 1));
    	}else{
    		if(hex[i] >= 'A' && hex[i] <= 'F'){
    			num = num + (hex[i] - 55) * int(pow(16, length - i - 1));
    		}else{
    			num = num + (hex[i] - 87) * int(pow(16, length - i - 1));
    		}

    	}
    	
    }
    return num;
}

void read_memory(char *file, int *mem, int *disk){
	ifstream fin(file);
	if(!fin){
		cout<<"open file failed !"<<endl;
	}
	char c;
	while(fin >> c){
		if(c == '~')
			break;
	}
	fin >> c;
	fin >> c;
	int pi = 0;//mem_index
	char cc[2];
	while(fin >> c){
		if(c == 'p')
			continue;
		if(c == ':'){
			while(fin >> cc){
				if(cc[0] == 'p')
					break;
				if(cc[0] == '~')
					break;
				mem[pi] = HextoDec(cc);
				pi++;
				cout<<"cc : "<<cc<<"  "<<"index of mem "<<pi - 1<<endl;
			}
		}
		if(c == '~')//three ~ are over
			break;

	}
	//fin >> c;
	while(fin >> c){
		if(c == '~')
			break;
	}
	fin >> c;
	fin >> c;
	pi = 0;
	strcpy(cc, " ");
	while(fin >> c){
		if(c == 'd')
			continue;
		if(c == ':'){
			while(fin >> cc){
				if(cc[0] == 'd')
					break;
				if(cc[0] == '~')
					break;
				disk[pi] = HextoDec(cc);
				pi++;
				cout<<"cc : "<<cc<<"  "<<"index of disk "<<pi - 1<<endl;
			}
		}
		cout<<"read disk over "<<endl;
		break;
		if(c == '~'){
			cout<<"break disk read is over "<<endl;
			break;
		}
	}

	fin.close();

}

void VMtoPM(int VM, char *vm, int *memory, int *disk, int p0){
	int pde_index, pte_index, offset;
	int pde_content, pte_content, content;
	cout<<"Virtual Address : "<< vm<<endl;
	cout<<"-->"<<"pde index : ";
	offset = VM & 31;
	pte_index = (VM & 992) >> 5;
	pde_index = (VM & 31744) >> 10;//OK
	cout<<pde_index<<"   "<<endl;
	pde_content = memory[32 * p0 + pde_index];
	cout<<"pde content : ( ";
	printf("0x%x", pde_content); 
	cout <<" valid : ";
	printf("%x", (pde_content & 128) >> 7);
	cout << "  pfn : ";
	printf("0x%x", pde_content & 127);
	cout <<" )"<<endl;
	if(((pde_content & 128) >> 7) == 0){
		cout<<"Unvalible   ....   !!!   "<<endl;
		return;
	}
	else{
		pte_content = memory[32 * (pde_content & 127) + pte_index];
		cout<<"--> pte_index : ";
		printf("0x%x   ", pte_index);
		cout<<"pte content : ( ";
		printf("0x%x", pte_content);
		cout<<" valid : "<<((pte_content & 128) >> 7)<<"  pfn : ";
		printf("0x%x", pte_content & 127);
		cout <<" )"<<endl;
		int pfn = pte_content & 127;
		if(((pte_content & 128) >> 7) == 0){//外存查找
			cout<<"--> To Disk Sector Address "<<" 0x";
			printf("%x\n", (pfn << 5) + offset);
			cout<<"--> value :";
			content = disk[32 * pfn + offset];
			printf("%02x\n", content);
		}else{
			cout<<"--> To Physical Address "<< " 0x";
			printf("%x\n", (pfn << 5) + offset);
			cout<<"--> value :";
			content = memory[32 * pfn + offset];
			printf("%02x\n", content);
		}
	}
}

int main(){
	char filename[] = "04-1-spoc-memdiskdata.md";
	int memory[4096];
	int disk[4096];//均用十进制进行存储
	char vm[5][5] = {"6653", "1c13", "6890", "0af6", "1e6f"};
	int VM[5] = {HextoDec("6653"), HextoDec("1c13"), HextoDec("6890"), HextoDec("0af6"), HextoDec("1e6f")};
	memset(memory, 0, sizeof(memory));
	memset(disk, 0, sizeof(disk)); 
	int PBDR = 3456;//0xd80
	int p0 = PBDR / pageSize; //PBDR对应的其实页基址
	int i, j;
	read_memory(filename, memory, disk);//读取文件，得到内存，硬盘
	int pde_index, pte_index, offset;
	int pde_content, pte_content, content;
	cout<<"for test ....."<<endl;
	/*
	cout<<"Virtual Address : "<<"0330"<<endl;
	cout<<"--> pde index : "<<((HextoDec("0330") & 31744) >> 10)<<endl;
	cout<<"pte index : "<<((HextoDec("0330") & 992) >> 5)<<endl;
	cout<<"offset : "<<(HextoDec("0330") & 31)<<endl;
	*/
	VMtoPM(HextoDec("0330"), "0330", memory, disk, p0);
	cout<<".........................."<<endl;
	for(i = 0; i < 5; i++){
		/*
		cout<<"Virtual Address : "<< vm[i]<<endl;
		cout<<"-->"<<"pde index : ";
		offset = VM[i] & 31;
		pte_index = (VM[i] & 992) >> 5;
		pde_index = (VM[i] & 31744) >> 10;//OK
		cout<<pde_index<<"   ";
		pde_content = memory[32 * p0 + pde_index];
		cout<<"pde content : ("<< pde_content <<"(10进制)  "<<"valid : "<<((pde_content & 128) >> 7)<< "pfn : "<<(pde_content & 127) << "10进制" << ")"<<endl;
		if(((pde_content & 128) >> 7) == 0){
			cout<<"unvalible .... "<<endl;
			continue;
		}
		else{
			pte_content = memory[32 * (pde_content & 127) + pte_index];
			cout<<"pte content : ("<<pte_content<<"（10进制）"<<"valid : "<<((pte_content & 128) >> 7)<<"pfn : "<<(pte_content & 127) << "10进制" <<")"<<endl;
			int pfn = pte_content & 127;
			if(((pte_content & 128) >> 7) == 0){//外存查找
				cout<<"--> To Disk Sector Address "<<((pfn << 5) + offset)<<"10进制   0x";
				printf("%x\n", (pfn << 5) + offset);
				cout<<"--> value :";
				content = disk[32 * pfn + offset];
				printf("%02x\n", content);
			}else{
				cout<<"--> To Physical Address "<< ((pfn << 5) + offset)<<"10进制   0x";
				printf("%x\n", (pfn << 5) + offset);
				cout<<"--> value :";
				content = memory[32 * pfn + offset];
				printf("%02x\n", content);
			}
		}*/
		VMtoPM(VM[i], vm[i], memory, disk, p0);
	}
	
	return 0;
} 
