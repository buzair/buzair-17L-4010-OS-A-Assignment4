# include <iostream>
# include <fstream>
# include <string>
# include <cstdlib>
# include <sstream>
using namespace std;

class FAT {
public:
	string name;
	string type;
	int start;
	FAT() {
		start = -2;
	}
	string getName() {
		return name;
	}
	void setName(string in) {
		name = in;
	}
	int getStart() {
		return start;
	}
	string getType() {
		return type;
	}
	void setType(string in) {
		type = in;
	}
};

int limitfat = 1000;
string VCB [1000];
string filee = "File", folderr = "Folder";
int maxTree = 8;
FAT fatdata[1000];
int countFCB = 0;
int curr = 0;
int curFol[8];
int spfolder = 8;
int limitfcb = 1000;
int fcb[1000];
int blocksize;

int removeFCB(string name, int in, string typ) {
	bool found = false;
	int qq = 1;
	do {
		string we = VCB[in];
		int p = 0;
		char ch = ' ';
		qq = 1;
		if (we.length() > 0) {
			for (; qq < we.length() && qq < blocksize; qq++) {
				ch = we[qq];
				if (ch == '|') {
					string qwe = we.substr(qq - 3, 3);
					p = atoi(qwe.c_str());
				}

				if (fatdata[p].name == name && fatdata[p].getType() == typ) {
					found = true;
					break;
				}
			}
		}
		if (found) {
			int a = p;
			ostringstream str1; 
			str1 << a; 
		  	string str = str1.str();
			if (str.length() == 3) {
				str = ("" + str + "|");
			}
			if (str.length() == 2) {
				str = ("0" + str + "|");
			}
			if (str.length() == 1) {
				str = ("00" + str + "|");
			}
			VCB[in] = VCB[in].substr(0, VCB[in].find(str)) + VCB[in].substr(VCB[in].find(str) + str.length(), VCB[in].length() - (VCB[in].find(str) + str.length()));
			return 1;
			break;
		}
		in = fcb[in];
	} while (in != -1 && !found);
	return -1;
}
void mkdir(string name, int in) {
	bool found = false;
	int qq = 1;
	int newsp = 1, retValue = -1;
	if (fcb[in] != -1) {
		while (fcb[in] != -1 && !found) {
			string we = VCB[in];
			int p = 0;
			char ch = ' ';
			qq = 1;
			if (we.length() > 0) {
				for (; qq < we.length() && qq < blocksize; qq++) {
					ch = we[qq];
					if (ch == '|') {
						string qwe = we.substr(qq - 3, 3);
						p = atoi(qwe.c_str());
					}
					if (fatdata[p].name == name && fatdata[p].getType() == folderr) {
						found = true;
						break;
					}
				}
			}
			if (found) {
				break;
			}
			in = fcb[in];
		}
	}
	string we = VCB[in];
	int p = 0;
	qq = 1;
	char ch = ' ';
	if (we.length() > 0) {
		for (; qq < we.length(); qq++) {
			ch = we[qq];
			if (ch == '|') {
				string qwe = we.substr(qq - 3, 3);
				p = atoi(qwe.c_str());
			}

			if (fatdata[p].name == name && fatdata[p].getType() == folderr) {
				found = true;
				break;
			}
		}
	}
	int space = 1;
	for (; space < limitfat; space++) {
		if (fcb[space] == -2)
			break;
	}
	if (space == limitfat) {
		cout << endl << "No Memory For This Folder" << endl;
		return ;
	}
	if (!found) {
		bool spacerem = false;
		for (; newsp < limitfat; newsp++) {
			if (fatdata[newsp].start == -2) {
				retValue = newsp;
				spacerem = true;
				break;
			}
		}
		if (spacerem) {
			fatdata[newsp].name = name;
			fatdata[newsp].start = space;
			fatdata[newsp].setType(folderr);
			fcb[space] = -1;
			countFCB++;
			if (VCB[in].length() + spfolder > blocksize) {
				int parentSpace = 1;
				for (; parentSpace < limitfat; parentSpace++) {
					if (fcb[parentSpace] == -2) {
						break;
					}
				}

				if (parentSpace == limitfat) {
					cout << "Last Block Of Memory";
					found = true;
				}

				fcb[in] = parentSpace;
				fcb[parentSpace] = -1;
				in = parentSpace;



				int a = newsp;
				ostringstream str1; 
				str1 << a; 
			  	string str = str1.str();

				if (str.length() == 3) {
					str = ("" + str + "|");
				}
				if (str.length() == 2) {
					str = ("0" + str + "|");
				}
				if (str.length() == 1) {
					str = ("00" + str + "|");
				}
				VCB[in] = VCB[in].substr(0, VCB[in].length()) + (str);
			}
			else {
				int a = newsp;
				ostringstream str1; 
				str1 << a; 
			  	string str = str1.str();

				if (str.length() == 3) {
					str = ("" + str + "|");
				}
				if (str.length() == 2) {
					str = ("0" + str + "|");
				}
				if (str.length() == 1) {
					str = ("00" + str + "|");
				}
				if (VCB[in][0] == '_')

					VCB[in] = str;
				else
					VCB[in] = VCB[in].substr(0, VCB[in].length()) + (str);
			}
		}
		else {
			cout << endl << "FAT Table Full" << endl;
		}
	}
	else {
		cout << endl << "Already A Folder With Same Name" << endl;
		return;
	}
	//return retValue;
}
int createFile(string name, int in) {
	bool found = false;
	int qq = 1;
	int ret = -1;
	int retValue = -1;
	if (fcb[in] != -1) {
		while (fcb[in] != -1 && !found) {


			string we = VCB[in];
			int p = 0;

			char ch = ' ';
			qq = 1;
			if (we.length() > 0) {
				for (; qq < we.length() && qq < blocksize; qq++) {
					ch = we[qq];
					if (ch == '|') {
						string qwe = we.substr(qq - 3, 3);
						p =atoi(qwe.c_str());
					}
					if (fatdata[p].name == name && fatdata[p].getType() == filee) {
						found = true;
						break;
					}
				}
			}
			if (found) {
				break;
			}
			in = fcb[in];
		}
	}
	string we = VCB[in];
	int p = 0;
	qq = 1;
	char ch = ' ';
	if (we.length() > 0) {
		for (; qq < we.length() && qq < blocksize; qq++) {
			ch = we[qq];
			if (ch == '|') {
				string qwe = we.substr(qq - 3, 3);
				p =atoi(qwe.c_str());
			}
			if (fatdata[p].name == name && fatdata[p].getType() == filee) {
				found = true;
				break;
			}
		}
	}
	int space = 1;
	for (; space < limitfat; space++) {
		if (fcb[space] == -2)
			break;
	}
	if (space == limitfat) {
		cout << "Memory Of Fat Is Full";
		found = true;
	}
	if (!found) {
		bool spacerem = false;
		int newsp = 1;
		for (; newsp < limitfat; newsp++) {
			if (fatdata[newsp].start == -2) {
				retValue = newsp;
				spacerem = true;
				break;
			}
		}
		if (spacerem) {

			fatdata[newsp].name = name;
			fatdata[newsp].start = space;
			fatdata[newsp].setType(filee);
			fcb[space] = -1;
			ret = newsp;
			if (VCB[in].length() + spfolder > blocksize) {
				int parentSpace = 1;
				for (; parentSpace < limitfat; parentSpace++) {
					if (fcb[parentSpace] == -2) {
						break;
					}
				}
				if (parentSpace == limitfat) {
					cout << "Last Block Of Memory";
					found = true;
				}
				fcb[in] = parentSpace;
				fcb[parentSpace] = -1;
				in = parentSpace;
				int a = newsp;
				ostringstream str1; 
				str1 << a; 
			  	string str = str1.str();

				if (str.length() == 3) {
					str = ("" + str + "|");
				}
				if (str.length() == 2) {
					str = ("0" + str + "|");
				}
				if (str.length() == 1) {
					str = ("00" + str + "|");
				}
				VCB[in] = VCB[in].substr(0, VCB[in].length()) + (str);

			}
			else {
				int a = newsp;
				ostringstream str1; 
				str1 << a; 
			  	string str = str1.str();

				if (str.length() == 3) {
					str = ("" + str + "|");
				}
				if (str.length() == 2) {
					str = ("0" + str + "|");
				}
				if (str.length() == 1) {
					str = ("00" + str + "|");
				}
				if (VCB[in][0] == '_')
					VCB[in] = str;
				else
					VCB[in] = VCB[in].substr(0, VCB[in].length()) + (str);
			}
		}
		else {
			cout << endl << "FAT Table Full" << endl;
		}

	}
	else {
		cout << endl << "Already A File With Same Name" << endl;
	}
}
void rmdir(string name, int in) {
	bool found = false;
	int qq = 1;
	int p = in;
	int save = in;
	if (fcb[in] != -1) {
		while (fcb[in] != -1 && !found) {
			string we = VCB[in];
			p = in;
			char ch = ' ';
			qq = 1;
			if (we.length() > 0) {
				for (; qq < we.length() && qq < blocksize; qq++) {
					ch = we[qq];
					if (ch == '|') {
						string qwe = we.substr(qq - 3, 3);
						p =atoi(qwe.c_str());
					}
					if (fatdata[p].name == name && fatdata[p].getType() == folderr) {
						found = true;
						break;
					}
				}
			}
			if (found) {
				break;
			}
			in = fcb[in];
		}
	}
	string we = VCB[in];
	qq = 1;
	char ch = ' ';
	if (we.length() > 0) {
		for (; qq < we.length() && qq < 97; qq++) {
			ch = we[qq];
			if (ch == '|') {
				string qwe = we.substr(qq - 3, 3);
				p = atoi(qwe.c_str());
			}

			if (fatdata[p].name == name && fatdata[p].getType() == folderr) {
				found = true;
				break;
			}
		}
	}
	int asd = VCB[fatdata[p].start].length();
	if (found && asd==0) {
		fcb[fatdata[p].start] = -2;
		fatdata[p].start = -2;
		removeFCB(name, save, folderr);
	}
	if (asd != 0) {
		cout << endl << "Folder Contain Items" << endl;
	}
	if (!found) {
		cout << endl << "No Folder With This Name" << endl;
	}
}
void readDataFile(string name, int in, string path)
{
	int ttt = fatdata[curFol[curr]].start;
	int p = 0;
	int ss = ttt;
	int qq = 0;
	int cc = curr;
        createFile(name,in);
	int found = -1;
	do {
		string we = VCB[ss];
		char ch = ' ';
		qq = 1;
		if (we.length() > 0) {
			for (; qq < we.length() && qq < blocksize; qq++) {
				ch = we[qq];
				if (ch == '|') {
					string qwe = we.substr(qq - 3, 3);
					p = atoi(qwe.c_str());
				}

				if (fatdata[p].name == name && fatdata[p].getType() == filee) {
					found = p;
					break;
				}

				if (ch == '_') {
					break;
				}

			}
		}

		ttt = fcb[ttt];
		ss = ttt;

	} while (ttt != -1);
        found=p;
	if (found == -1) {
		cout << endl << "No File Was Found With That Name" << endl;
	}
	else {
		ifstream file;  
                file.open(path.c_str());
		if (!file.is_open()) {
			cout << endl << "Enter Path Can Not Be Opened" << endl;
		}
		else {
			int now = found;
			int preBlock = now;
			bool hasSpace = true;
			string read;
			while (!file.eof()) {
				getline(file, read);
				while (read.length() != 0) {
					if (VCB[now][0] == '_')
						VCB[now] = read.substr(0, blocksize);
					else {
						VCB[now].append(read.substr(0, blocksize));
					}
					if (read.length() > blocksize-1)
						read = read.substr(blocksize, read.length() - blocksize);
					else
						read = "";
					int space = 1;
					for (; space < limitfat; space++) {
						if (fcb[space] == -2)
							break;
					}
					if (space == limitfat) {
						hasSpace = false;
						break;
					}
					preBlock = now;
					fcb[now] = space;
					fcb[space] = -1;
					now = space;
					VCB[now] = "";
				}
				if (!hasSpace) {
					cout << endl << "Not More Space" << endl;;
					break;
				}

				if (VCB[preBlock].length() < blocksize) {
					VCB[preBlock] = VCB[preBlock] + '\n';
				}
				else {
					VCB[now] = '\n';
				}
			}
		}
		file.close();
	}
}
void ls(int t) {
	int ttt = t;
	int p = 0;
	int ss;
	int qq = 0;
	int cc = curr;
	do {
		ss = ttt;
		string we = VCB[ss];
		int p = 0;
		char ch = ' ';
		qq = 1;
		if (we.length() > 0) {
			for (; qq < we.length() && qq < blocksize; qq++) {
				ch = we[qq];
				if (ch == '|') {
					string qwe = we.substr(qq - 3, 3);
					p =atoi(qwe.c_str());
					cout << fatdata[p].name << "'" << fatdata[p].getType() << "'";
				}
			}
		}
		ttt = fcb[ttt];
	} while (ttt != -1);
	cout << endl;
}
void printFile(string name, int in)
{
	int ttt = fatdata[curFol[curr]].start;
	int p = 0;
	int ss = ttt;
	int qq = 0;
	int cc = curr;
	int found = -1;
	do {
		string we = VCB[ss];
		char ch = ' ';
		qq = 1;
		if (we.length() > 0) {
			for (; qq < we.length() && qq < blocksize; qq++) {
				ch = we[qq];
				if (ch == '|') {
					string qwe = we.substr(qq - 3, 3);
					p =atoi(qwe.c_str());
				}
				if (fatdata[p].name == name && fatdata[p].getType() == filee) {
					found = p;
					break;
				}
				if (ch == '_') {
					break;
				}
			}
		}
		ttt = fcb[ttt];
		ss = ttt;
	} while (ttt != -1);

	if (found == -1) {
		cout << endl << "No File Was Found With That Name" << endl;
	}
	else {
		int now = fatdata[found].start;
		cout << endl;
		cout << " File Name:  " << fatdata[p].name << endl;
		while (now != -1) {
			cout << VCB[now];
			now = fcb[now];
		}
		cout<<endl;
	}
}
void drawBinary() {
	cout << endl << "      Volume Map   " << endl;
	for (int i = 0; i < limitfat; i++) {
		if (fcb[i] != -2) {
			cout << "1";
		}
		else {
			cout << "0";
		}
		if ((i + 1) % 50 == 0)
			cout << endl;
	}
	cout << endl;
}
int main() {
	curFol[0] = 0;
	for (int i = 0; i < limitfat; i++) {
		fcb[i] = -2;
		VCB[i][0] = '_';
	}
	for (int i = 0; i < limitfcb; i++) {
		fatdata->start = -2;
	}
	string nn1;
	int bs1;
	cout<<"Enter Name : ";
	cin>>nn1;
	cout<<"Enter blocksize : ";
	cin>>bs1;
	blocksize=bs1;
	fatdata[countFCB].name = nn1;
	fatdata[countFCB].start = 0;
	fatdata[countFCB].setType(folderr);
	fcb[0] = -1;
	countFCB++;
	string input="";
	cout << endl << "Enter Help for Information "<<endl;
	while (true) {
		if (input == "Help") {
			cout << endl << "Welcome To Help" << endl;
			cout << "1. Input 'ls' for ls Command"<<endl;
			cout << "2. Input 'rm' <folderName> for deleting Folder"<<endl;
			cout << "3. Input 'mkdir' <foldername>  for creating Folder"<<endl;
			cout << "4. Input 'cd' <folderName> <path> for next Folder"<<endl;
			cout << "5. Input 'importFile' <filename> <Path Of File With Name> for importing File"<<endl;
			cout << "6. Input 'allDetails' for all directories VCB"<<endl;
			cout << "7. Input 'exit' for Exit"<<endl;
		}
		do {
			int var = -1;
			do {
				var++;
				cout << fatdata[curFol[var]].name << "\\";
			} while (var < curr);
		
			getline(cin,input);
		} while (!input.compare(""));
		int len = 0;
		for (; len < input.length(); len++) {
			if (input[len] != ' ')
				break;
		}
		input = input.substr(len, input.length() - len);
		int tell = input.find(' ');
		if (tell == -1) {
			input = input.substr(len, input.length() - len) + " ";
			tell = input.find(' ');
		}
		string cmd;
		if (tell != -1) {
			cmd = input.substr(0, tell);
			input = input.substr(tell, input.length() - tell);
		}
		else
			cmd = input;
		if (cmd == "Help") {
			input = "Help";
		}
		if (cmd == "volumeMap") {
			int l = 0;
			for (; l < input.length(); l++) {
				if (input.length() > 0)
				if (input[l] == ' ')
					break;
			}
			for (; l < input.length(); l++) {
				if (input.length() > 0)
				if (input[l] != ' ')
					break;
			}
			input = input.substr(l, input.length() - l);
			int in = fatdata[curFol[curr]].start;
			string name = input;
			if (name == "") {
				drawBinary();
			}
			else
				cout << endl << "Wrong Syntax" << endl;
		}
		//mkdir
		else if (cmd == "mkdir") {
			int l=0;
			for (; l < input.length(); l++) {
				if (input.length() > 0)
				if (input[l] == ' ')
					break;
			}
			for (; l < input.length(); l++) {
				if (input.length() > 0)
				if (input[l] != ' ')
					break;
			}
			input = input.substr(l, input.length() - l);
			int in = fatdata[curFol[curr]].start;
			string name = input;
			if (name != "") {
				mkdir(name, in);
			}
			else
				cout << endl << "Wrong Syntax" << endl;

		}
		// cd
		else if (cmd == "cd") {
			int ttt = fatdata[curFol[curr]].start;
			//cout<<"ttt   :"<<ttt<<endl;;
			int p = 0;
			int ss = ttt;
			int qq = 0;
			int cc = curr;
			int found = -1;
			int l = 0;
			for (; l < input.length(); l++) {
				if (input.length() > 0)
				if (input[l] == ' ')
					break;
			}
			for (; l < input.length(); l++) {
				if (input.length() > 0)
				if (input[l] != ' ')
					break;
			}
			input = input.substr(l, input.length() - l);
			int in = fatdata[curFol[curr]].start;
			string name = input;
			if (name != "") {
				if (cc != maxTree) {
					do {
						string we = VCB[ss];
					//	cout<<"we :   "<<we<<"    "<<endl;
						int p = 0;
						char ch = ' ';
						qq = 1;
						if (we.length() > 0) {
							for (; qq < we.length() && qq < blocksize; qq++) {
								ch = we[qq];
								if (ch == '|') {
									string qwe = we.substr(qq - 3, 3);
									//cout<<"qwe :   "<<qwe<<"    "<<endl;
									p =atoi(qwe.c_str());
								}
								if (fatdata[p].name == name && fatdata[p].getType() == folderr) {
									found = p;
									break;
								}
								if (ch == '_') {
									break;
								}
							}
						}
						ttt = fcb[ttt];
						ss = ttt;

					} while (ttt != -1);

					if (found == -1) {
						cout << endl << "No Folder With This Name" << endl;
					}
					else {
						curr++;
						curFol[curr] = found;
					}
				}
				else {
					cout << endl << "Max Tree Is " << maxTree << endl;
				}
			}
			else
				cout << endl << "Wrong Syntax" << endl;
		}
		else if (cmd == "rm") {
			int l = 0;
			for (; l < input.length(); l++) {
				if (input.length() > 0)
				if (input[l] == ' ')
					break;
			}
			for (; l < input.length(); l++) {
				if (input.length() > 0)
				if (input[l] != ' ')
					break;
			}
			input = input.substr(l, input.length() - l);
			int in = fatdata[curFol[curr]].start;
			string name = input;
			if (name != "") {
				rmdir(name, in);
			}
			else
				cout << endl << "Wrong Syntax" << endl;
		}
		else if (cmd == "importFile") {
			int ss = fatdata[curFol[curr]].start;

			int l = 0;
			for (; l < input.length(); l++) {
				if (input.length() > 0)
				if (input[l] == ' ')
					break;
			}
			for (; l < input.length(); l++) {
				if (input.length() > 0)
				if (input[l] != ' ')
					break;
			}
			input = input.substr(l, input.length() - l);
			int in = fatdata[curFol[curr]].start;
			l = input.length();
			for (; l != 0; l--) {
				if (input.length() > 0)
				if (input[l] != ' ')
					break;
			}
			input = input.substr(0, l);
			l = input.length() - 1;
			for (; l > 0; l--) {
				if(input.length()>0)
					if (input[l] == ' ')
						break;
			}
			string path = "";
			if (input.length() > 0)
				path = input.substr(l);

			int kk = 0;
			for (; kk < path.length(); kk++) {
				if (path[kk] != ' ')
					break;
			}
			path = path.substr(kk);

			string name = input.substr(0, l);
			if (name != "" && path != "") {
				readDataFile(name, ss, path);
			}
			else {
				cout << endl << "Wrong Syntax" << endl;
			}
		}
		else if (cmd == "ls") {
			int l = 0;
			for (; l < input.length(); l++) {
				if (input.length() > 0)
				if (input[l] == ' ')
					break;
			}
			for (; l < input.length(); l++) {
				if (input.length() > 0)
				if (input[l] != ' ')
					break;
			}
			input = input.substr(l, input.length() - l);
			int in = fatdata[curFol[curr]].start;
			string name = input;			
			if (name == "") {
				ls(in);
			}
			else
				cout << endl << "Wrong Syntax" << endl;
		}
		else if (cmd == "allDetails") {
			int l = 0;
			for (; l < input.length(); l++) {
				if (input.length() > 0)
				if (input[l] == ' ')
					break;
			}
			for (; l < input.length(); l++) {
				if (input.length() > 0)
				if (input[l] != ' ')
					break;
			}
			input = input.substr(l, input.length() - l);
			int in = fatdata[curFol[curr]].start;
			string name = input;
			if (name == "") {
				for (int i = 0; i < limitfcb; i++) {
					if (fatdata[i].getType() == folderr && fatdata[i].start != -2) {
						cout << endl;
						cout << endl << "Folder Name  :  " << fatdata[i].name << endl;
						ls(fatdata[i].start);
						cout << endl;
					}
				}
			}
			else
				cout << endl << "Wrong Syntax" << endl;
		}
		else if (cmd == "viewData") {
			int l = 0;
			for (; l < input.length(); l++) {
				if (input.length() > 0)
				if (input[l] == ' ')
					break;
			}
			for (; l < input.length(); l++) {
				if (input.length() > 0)
				if (input[l] != ' ')
					break;
			}
			input = input.substr(l, input.length() - l);
			int in = fatdata[curFol[curr]].start;
			string name = input;
			if (name != "") {
				printFile(name, in);
			}
			else
				cout << endl << "Wrong Syntax" << endl;
		}
		else if (cmd == "exit") {
			cout << endl << "Exiting..." << endl;
			break;
		}
		else
			cout<<"Wrong Input"<<endl;
	}
}
