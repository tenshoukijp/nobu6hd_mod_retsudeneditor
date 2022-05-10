#pragma once

#include <vector>
#include <string>

#include "CnvHankakuZenkaku.h"
#include "OnigRegex.h"

#include "ls11_mod.h"
#include "DotNetTestToMultiByte.h"


using namespace System;
using namespace System::Text;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Text::RegularExpressions;


char *szBushouTargetFileName = "bfile[1-26].dat";

// ���傤�� vBufBushouDecodedData[0]=�P�Ԗڂ̕����t�@�C���̃f�[�^��AvBufBushouDecodedData[1]=�Q�Ԗڂ̕����t�@�C���̃f�[�^��A�݂����Ȋ���
vector<vector<byte>> vBufBushouDecodedData;


ref class BushouData {
public:
	List<String^>^ lstStrFullName; // ����
	List<String^>^ lstStrRubyYomi; // ��݂���
	List<String^>^ lstStrBornEtc;  // ���N
	List<String^>^ lstStrRetsuden; // ��`

public:
	BushouData() {
		lstStrFullName = gcnew List<String^>();
		lstStrRubyYomi = gcnew List<String^>();
		lstStrBornEtc  = gcnew List<String^>();
		lstStrRetsuden = gcnew List<String^>();

		BushouData_Import();
	}

	//  �������u������
	std::string StrReplace( std::string String1, std::string String2, std::string String3 )
	{
		std::string::size_type  Pos( String1.find( String2 ) );

		while( Pos != std::string::npos )
		{
			String1.replace( Pos, String2.length(), String3 );
			Pos = String1.find( String2, Pos + String3.length() );
		}

		return String1;
	}

	std::string HankakuKatakanaToZentakuKatakana(std::string szBufOriginal) {
		// ��������ƃo�b�t�@������Ȃ��Ȃ�̂ŁA�������Ȃ�
		// ��HD��p�̃p�b�`
		return szBufOriginal;

		/*


		std::string szBufCurrent = szBufOriginal;

		Matches matches;

		// ���̓���ȋL���͓V�ċL�ɂ����āA���p�J�^�J�i��S�p�J�^�J�i�ɂ���Ƃ����Ӗ�
		for (int i=0; OnigMatch(szBufCurrent, "^(.*?)\x1b\x4b(.*?)\x1b\x48(.*?)$", &matches) && i<(int)szBufCurrent.size(); i++ ) { 
			// ��`����v�f�𒊏o����B
			if ( !matches[2].empty() ) {
				char szBufZenKatakana[256] = "";
				hankana2zenkana((unsigned char *)matches[2].c_str(), (unsigned char *)szBufZenKatakana);
				szBufZenKatakana[255] = NULL; // ����ȏC�����Ă��\�����Ȃ��悤�O�̂���

				szBufCurrent = matches[1] + szBufZenKatakana + matches[3];
			}
		}

		// �Ō�̕���L�����Ȃ��܂܍s�����}����ꍇ������B
		if ( OnigMatch(szBufCurrent, "^(.*?)\x1b\x4b(.+)$", &matches) ) {

			// ��`����v�f�𒊏o����B
			if ( !matches[2].empty() ) {
				char szBufZenKatakana[256] = "";
				hankana2zenkana((unsigned char *)matches[2].c_str(), (unsigned char *)szBufZenKatakana);
				szBufZenKatakana[255] = NULL; // ����ȏC�����Ă��\�����Ȃ��悤�O�̂���

				szBufCurrent = matches[1] + szBufZenKatakana;
			}
		}

		return szBufCurrent;

		*/
	}

	// ���O�n��Unmanaged��Managed�ւƒ��o�B
	void NameAssignManagedData(char *pUnmanagedLine) {
		string line = string(pUnmanagedLine);
		Matches matches;
		if ( OnigMatch(line, "^(.*?) \x1b\x6b(.*?)\x1b\x48 (.*?)\x05\x05\x05$", &matches) ) {

			// ���]�䕔�ƍ��@�䕔�̓��ʏ���
			matches[1] = StrReplace(matches[1], "\xEC\x8C\xEC\x8D\xEC\x8E", "���@�䕔" );
			matches[1] = StrReplace(matches[1], "\xEC\x8F\xEC\x8D\xEC\x8E", "���@�䕔" );

			// ���p�ˑS�p
			char szBufZenKatakana[256] = "";
			hankana2zenkana((unsigned char *)matches[1].c_str(), (unsigned char *)szBufZenKatakana);
			szBufZenKatakana[255] = NULL; // ����ȏC�����Ă��\�����Ȃ��悤�O�̂���

			String^ strFullName = gcnew String(szBufZenKatakana);
			lstStrFullName->Add( strFullName );

			String^ strRubyYomi = gcnew String(matches[2].c_str());
			lstStrRubyYomi->Add( strRubyYomi );

			String^ strBornEtc  = gcnew String(matches[3].c_str());
			lstStrBornEtc->Add( strBornEtc );
		}
	}

	// ���Add�łł͂Ȃ��āA�㏑���š�� �Ђǂ��\�[�X�ɂȂ��Ă���orz
	BOOL NameOverrideManagedData(char *pUnmanagedLine, int iSelectedIndex) {
		string line = string(pUnmanagedLine);
		Matches matches;
		if ( OnigMatch(line, "^(.*?) \x1b\x6b(.*?)\x1b\x48 (.*?)\x05\x05\x05(.*?)\x05\x05\x05$", &matches) ) {
			// ���]�䕔�ƍ��@�䕔�̓��ʏ���
			matches[1] = StrReplace(matches[1], "\xEC\x8C\xEC\x8D\xEC\x8E", "���@�䕔" );
			matches[1] = StrReplace(matches[1], "\xEC\x8F\xEC\x8D\xEC\x8E", "���@�䕔" );

			// �I�����Ă���Ƃ���ɏ㏑��
			String^ strFullName = gcnew String(matches[1].c_str());
			lstStrFullName[iSelectedIndex] = strFullName;

			String^ strRubyYomi = gcnew String(matches[2].c_str());
			lstStrRubyYomi[iSelectedIndex] = strRubyYomi;

			String^ strBornEtc  = gcnew String(matches[3].c_str());
			lstStrBornEtc[iSelectedIndex] = strBornEtc;

			return TRUE;
		}
		return FALSE;
	}

	// ��`�n��Unmanaged��Managed�ւƒ��o�B
	String^ RetsudenAssignManagedData(char *pUnmanagedLine) {

		string szBufCurrent = string(pUnmanagedLine);

		szBufCurrent = HankakuKatakanaToZentakuKatakana(szBufCurrent);

		Matches matches;

		// �Ō�̏I���L���ȍ~�͏����B
		if ( OnigMatch(szBufCurrent, "^(.*?)\x05\x05\x05$", &matches) ) {

			if ( !matches[1].empty() ) {
				// �c��́A���p�J�^�J�i��S�p�Ђ炪�Ȃ�
				char szBufZenHiragana[256] = "";
				hankana2zenhira((unsigned char *)matches[1].c_str(), (unsigned char *)szBufZenHiragana);
				szBufZenHiragana[255] = NULL; // ����ȏC�����Ă��\�����Ȃ��悤�O�̂���

				string strZenHiragana = string(szBufZenHiragana);

				// ���s�I�Ӗ������̂��̂����ȋL���ɒu��������B�������̂��߁B
				for (int i=0; OnigMatch(strZenHiragana, "^(.*?)\n(.+)", &matches) && i<(int)strZenHiragana.size(); i++ ) { 
					// ��`����v�f�𒊏o����B
					if ( !matches[1].empty() ) {
						strZenHiragana = matches[1] + "<r><n>" + matches[2];
					}
				}

				// ����ȋL�����A��`�G�f�B�^��ł̌����ڂ̉��s�ւƕϊ��B
				for (int i=0; OnigMatch(strZenHiragana, "^(.*?)<r><n>(.+)", &matches) && i<(int)strZenHiragana.size(); i++ ) { 
					// ��`����v�f�𒊏o����B
					if ( !matches[1].empty() ) {
						strZenHiragana = matches[1] + "\r\n" + matches[2];
					}
				}
				return gcnew String(strZenHiragana.c_str());
			}
		}

		return gcnew String("");
	}



	// bfile*.dat���z��ւƊi�[�B
	int BushouData_Import() {
		if (!System::IO::File::Exists(gcnew String("bfile1.dat"))) {
			System::Windows::Forms::MessageBox::Show("bfile*.dat��ǂݍ��߂܂���", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
		}

		int iRetsudenNo = -1;

		vBufBushouDecodedData.resize(26);

		for (int ifile = 0; ifile < (int)vBufBushouDecodedData.size(); ifile++) {
			char szFileName[MAX_PATH] = "";
			sprintf(szFileName, "bfile%d.dat", ifile + 1);
			FILE *fp = fopen(szFileName, "rb");

			// �t�@�C���T�C�Y
			fpos_t fsize = 0;
			fpos_t fsizeb = fseek(fp, 0, SEEK_END);
			fgetpos(fp, &fsize);

			// ���ւƖ߂�
			rewind(fp);

			byte buf[20000]; // �������̂��߁Anew���Ȃ��B

							 // �R�s�[
			fread(buf, (size_t)fsize, 1, fp);

			// 95�łƈ���āA�擪�Ɂu64 00�v�Ƃ����悤�ɑS����100�f�[�^������Ƃ�����񂪕t���Ă���B
			WORD *pDataAddress = (WORD *)buf;
			WORD iDataCount = pDataAddress[0]; // �擪�̓f�[�^�̌�
			for (int i = 1; i <= iDataCount; i++) {
				pDataAddress[i] = pDataAddress[i] - 2; // �S���A�h���X����Q������
			}

			vector<byte> vdata(&buf[2], &buf[fsize]); // �ŏ���WORD�P����95�łɂ͖����J�E���g���Ȃ̂ŃR�s�[���Ȃ��B
			vBufBushouDecodedData[ifile] = vdata;
		}

		// �f�R�[�h�����f�[�^��(�����I)�t�@�C���ł܂킵�Ă����B0�Ԃ͑Ώۂł͂Ȃ��B�����f�[�^��1�Ԃ���B
		int ivBufDecodedDataSize = vBufBushouDecodedData.size();

		// 1�Ԗڂ���X�^�[�g���Ă���!! ����!!
		for ( int ifile = 0; ifile < (int)ivBufDecodedDataSize ; ifile++ ) {

			// ���傤�� vSplittedData[0]=�P�Ԗڂ̗v�f�̃f�[�^��AvSplittedData[1]=�Q�Ԗڂ̗v�f�̃f�[�^��A�݂����Ȋ���
			vector<vector<byte>> vSplittedData;

			ls11_SplitData( vBufBushouDecodedData[ifile], &vSplittedData );

			int ivSplittedDataSize = vSplittedData.size();

			// �e�v�f���Ƃɥ�� �܂����O�A�ǂ݉��� ���v�N�̗�
			for ( int ielem=0; ielem < (int)ivSplittedDataSize; ielem++ ) {
				if ( ielem%2==0 ) { iRetsudenNo++; }

				if ( iRetsudenNo < 0 ) { iRetsudenNo = 0; }; // �ň��̂��߂̔ԕ�

				// �������ǂ݉����Ƃ�
				if ( ielem%2==0 ) {

					int datasize = vSplittedData[ielem].size();
					if ( datasize > 0 ) {

						char *bytedata = new char[datasize+1];
						memcpy( bytedata, &vSplittedData[ielem][0], datasize );
						bytedata[datasize] = NULL; // �ԕ�

						NameAssignManagedData( bytedata ); // �}�l�[�W�h�f�[�^�ւƑ��

						delete bytedata;
					}


				// ��͗�`�B
				} else { 

					string szBufCurrent;

					int datasize = vSplittedData[ielem].size();
					if ( datasize > 0 ) {

						char *bytedata = new char[datasize+1];
						memcpy( bytedata, &vSplittedData[ielem][0], datasize );
						bytedata[datasize] = NULL; // �ԕ�
						szBufCurrent = string(bytedata); 

						String^ strRetsuden = RetsudenAssignManagedData( bytedata ); // �}�l�[�W�h�f�[�^�ւƑ��
						lstStrRetsuden->Add( strRetsuden );

						delete bytedata;
					}

				}
			}
		}

		// ���v��`����Ԃ��B
		return iRetsudenNo+1;
	}

	// �Ƃ���P�l�̕����f�[�^�̏o�͗p�o�C�i���f�[�^�𓾂�
	std::string GetOneBushouExportData(int iSelectedIndex) {
		String^ managedData = String::Format("{0} \x1b\x6b{1}\x1b\x48 {2}\x05\x05\x05{3}\x05\x05\x05", lstStrFullName[iSelectedIndex], lstStrRubyYomi[iSelectedIndex], lstStrBornEtc[iSelectedIndex], lstStrRetsuden[iSelectedIndex]);

		return GetOutputFormatNormalizeData(managedData);

	}

	std::string GetOutputFormatNormalizeData(String^ managedData) {
		// .NET�^�C�v��String^ �� �}���`�o�C�g��C++�^std::string��
		std::string unmanagedData = DotNetStringToMultiByte(managedData);

		Matches matches;
		// ����ȋL�����A��`�G�f�B�^��ł̌����ڂ̉��s�ւƕϊ��B
		for (int i=0; OnigMatch(unmanagedData, "^(.*?)\r\n(.+)", &matches) && i<(int)unmanagedData.size(); i++ ) { 
			// ��`����v�f�𒊏o����B
			if ( !matches[1].empty() ) {
				unmanagedData = matches[1] + "\x0A" + matches[2];
			}
		}

		return unmanagedData;
	}

	// �Ƃ���P�l�̕����f�[�^���t�@�C���������
	BOOL GetOneBushouImportData(String^ managedFilePath, int iSelectedIndex) {
		std::string unmanagedFilePath = DotNetStringToMultiByte(managedFilePath);

		FILE *fp = fopen( unmanagedFilePath.c_str(), "rb");
		if ( fp ) {
			char pUnmanagedLine[512] = "";
			fread( pUnmanagedLine, sizeof(pUnmanagedLine)-1, 1, fp); // ��C�ɓǂݍ���
			fclose(fp);


			string line = string(pUnmanagedLine);
			Matches matches;
			if ( OnigMatch(line, "^(.*?) \x1b\x6b(.*?)\x1b\x48 (.*?)\x05\x05\x05(.*?\x05\x05\x05$)", &matches) ) {

				BOOL ret1 = NameOverrideManagedData(pUnmanagedLine, iSelectedIndex);

				if (!ret1) {
					System::Windows::Forms::MessageBox::Show("�f�[�^�̎捞�Ɏ��s���܂����B", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
					return FALSE;
				}

				// ��`���������n��
				String^ strRetsuden = RetsudenAssignManagedData((char *)matches[4].c_str());

				lstStrRetsuden[iSelectedIndex] = strRetsuden;

				System::Windows::Forms::MessageBox::Show("�f�[�^����荞�݂܂����B�B", "����", MessageBoxButtons::OK, ::MessageBoxIcon::Information);
				return TRUE;
			}

		} else {
			System::Windows::Forms::MessageBox::Show("�t�@�C���̃I�[�v���Ɏ��s���܂����B", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
			return FALSE;
		}

		return FALSE;
	}

	BOOL AllSaveToBFileN6P() {
		vBufBushouDecodedData.clear(); // ��[�S����

		// ������`�S�Ăɂ��ĥ��
		int iFileCnt = (int)ceil((double)lstStrFullName->Count / 50.0); // ���z�t�@�C��(��������)�����J�E���g���B�P������50�Ȃ̂ŁB

		// ���ꂼ��̕����ԍ��̊J�n�ԍ��𑗂�Ȃ���A���z�t�@�C���t�@�C�����J��Ԃ��BvBufBushouDecodedData �ɕt���������Ă䂭�B
		for ( int f=0; f<iFileCnt; f++) {
			MakeSplittedDataToJoindData( f*50 );
		}

		int result = 0;
		for (int ifile = 0; ifile < (int)vBufBushouDecodedData.size(); ifile++) {
			char szFileName[MAX_PATH] = "";
			sprintf(szFileName, "bfile%d.dat", ifile + 1);
			FILE *fp = fopen(szFileName, "wb");
			if (!fp) {
				result = -1;
			}
			byte tmpBuf[20000];
			int iDataSize = vBufBushouDecodedData[ifile].size();
			memcpy(tmpBuf, vBufBushouDecodedData[ifile].data(), iDataSize);
			fwrite(tmpBuf, iDataSize, 1, fp);
			fclose(fp);
		}

		if ( result == 0 ) {
			System::Windows::Forms::MessageBox::Show( gcnew String(szBushouTargetFileName) + "�ւƏo�͂��܂����B", "����", MessageBoxButtons::OK, ::MessageBoxIcon::Information);
			return TRUE;
		} else {
			System::Windows::Forms::MessageBox::Show( "�\�����ʃG���[���������܂����B", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
			return FALSE;
		}
	}

	// �P�̉��z�t�@�C��(�������ꂽ���̑���)����������ɍ��o���B
	void MakeSplittedDataToJoindData(int iStartIndex) {
		vector<vector<byte>> vSplittedData; // 50 ��l����������ł䂭�f�[�^
		vector<byte> vDstJoinedData; // 50 ��l����������ł䂭�f�[�^

		// 50�l��
		for ( int i=iStartIndex; (i < iStartIndex + 50) && (i < lstStrFullName->Count); i++ ) {
			vSplittedData.push_back( GetNameLineData(i) );
			vSplittedData.push_back( GetRetsudenLineData(i) );
		}


		// �����������̂����ւƖ߂��B���P�t�@�C������������
		ls11_JoinData(vSplittedData, &vDstJoinedData);

		// �擪�Ƀf�[�^�������S�̂̃A�h���X�𑀍삷��B
		int iDataByteSize = vDstJoinedData.size();
		int iAllDataByteSize = iDataByteSize + 2;
		byte tmpBuf[20000] = { 0 };
		memcpy(tmpBuf +2, vDstJoinedData.data(), iDataByteSize);
		int iAddress = (int)tmpBuf + 2;
		WORD *wAddress = (WORD *)iAddress;

		int iDataSplittedCount = vSplittedData.size(); // ��������Ă����f�[�^��
		for (int i = 0; i < iDataSplittedCount; i++) {
			wAddress[i] = wAddress[i] + 2; // �Q�{�ƕ��A�擪�Ƀ^�X�\��Ȃ̂ŁA�A�h���X�����炵�Ă���
		}
		tmpBuf[0] = (byte)iDataSplittedCount; // �擪�Ƀf�[�^�̌���ݒ�
		vDstJoinedData.clear();

		vector<byte> vNewJoindData(&tmpBuf[0], &tmpBuf[iAllDataByteSize]);
		// �����S�̃f�[�^�ɉ�����B
		vBufBushouDecodedData.push_back(vNewJoindData);
	}

	// �Ƃ���P�l���̕����̖��O���N�n�̃f�[�^
	vector<byte> GetNameLineData(int iTargetIndex) {

		String^ format = String::Format("{0} \x1b\x6b{1}\x1b\x48 {2}\x05\x05\x05", lstStrFullName[iTargetIndex], lstStrRubyYomi[iTargetIndex], lstStrBornEtc[iTargetIndex]);

		std::string unmanagedData = GetOutputFormatNormalizeData(format);

		char *psz = (char *)unmanagedData.c_str();

		// ��C��char[]��vector<byte>�ւƃR�s�[
		vector<byte> line( &psz[0], &psz[strlen(psz)] );

		return line;
	}

	// �Ƃ���1�l���̕����̗�`�Ł[��
	vector<byte> GetRetsudenLineData(int iTargetIndex) {
		String^ format = String::Format("{0}\x05\x05\x05", lstStrRetsuden[iTargetIndex]);

		std::string unmanagedData = GetOutputFormatNormalizeData(format);

		char *psz = (char *)unmanagedData.c_str();

		// ��C��char[]��vector<byte>�ւƃR�s�[
		vector<byte> line( &psz[0], &psz[strlen(psz)] );

		return line;

	}

};