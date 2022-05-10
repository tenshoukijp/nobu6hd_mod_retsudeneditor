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


char *szCastleTargetFileName = "sfile[1-5].dat"; // ��

// ���傤�� vBufCastleDecodedData[0]=�P�Ԗڂ̕����t�@�C���̃f�[�^��AvBufCastleDecodedData[1]=�Q�Ԗڂ̕����t�@�C���̃f�[�^��A�݂����Ȋ���
vector<vector<byte>> vBufCastleDecodedData;


struct CastleType {
	char binAppearanceScenario[100]; // �o��V�i���I�o�C�i��
	char szFullName[32];
	char szRubyYomi[32];
};
struct CastleTypeRetsuden {
	char szRetsuden[512];
};


vector<vector<CastleType>> vCastleInfoListData; // ����\���̂������ɂȂ��ăl�X�g��������
vector<vector<CastleTypeRetsuden>> vCastleRetsudenInfoListData; // ����\���̂������ɂȂ��ăl�X�g��������

ref class CastleData {
public:
	List<String^>^ lstStrFullName; // ����
	List<String^>^ lstStrRubyYomi; // ��݂���
	List<String^>^ lstStrRetsuden; // ��`
	List<String^>^ lstStrBornEtc;  // ��`�̌��ɂ���悭�킩��Ȃ��f�[�^

public:
	CastleData() {
		lstStrFullName = gcnew List<String^>();
		lstStrRubyYomi = gcnew List<String^>();
		lstStrBornEtc  = gcnew List<String^>();
		lstStrRetsuden = gcnew List<String^>();

		CastleData_Import();
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
		string nestline = string(pUnmanagedLine);

		// �P�̃��C���ɕ����̏邪�܂܂�Ă���ꍇ������B�ő�ŃV�i���I���ł���U���܂܂��B
		Matches matches;

		vector<CastleType> nest;

		for ( int i=0; i<6; i++ ) {

			// �����̏邪�P�Ɏ��܂��Ă���ꍇ�A���̂悤�Ȍ`�B\x05���Q�ȏ゠��͂��B
			if ( OnigMatch(nestline, "^(.+)\x05\x05\x05(.*?)\x05\x05\x05$", &matches ) ) {

				string prev = matches[1]; // �O�ɂ������Ă���c��̏�
				string last = matches[2]; // �Ō�ɂ������Ă����(���)
				CastleType ct = {NULL};


				// �O���̊���Ȃ��́u�o��V�i���I�v�̃V�O�i���ŁA=\d���o��V�i���I�̔ԍ�
				if ( OnigMatch(last, "^(\x01.*=\x25\\d)?(.*?)\x20\x1b\x6b(.*?)$", &matches) ) {
					
					// �������͕����l�X�g�̍Ō�̂P�i���C���̂��́j�͑��݂����A����ȊO�͂������Ă���B
					// matches[1]==""�Ƃ������Ƃ́u�������Ă�����̗̂]��̃V�i���I�v�ł��邱�Ƃ��Ӗ�����B
					strcpy( ct.binAppearanceScenario, matches[1].c_str() );

					// �閼�͐�΂ɑ��݂��Ȃ���΂Ȃ�Ȃ�
					// ���p�J�^�J�i���S�p�Ђ炪�Ȃ̗�̂��
					string strFullName = HankakuKatakanaToZentakuKatakana(matches[2].c_str());

					// �閼�͐�΂ɑ��݂��Ȃ���΂Ȃ�Ȃ�
					strcpy( ct.szFullName, strFullName.c_str() );

					// �閼�͐�΂ɑ��݂��Ȃ���΂Ȃ�Ȃ�
					strcpy( ct.szRubyYomi, matches[3].c_str() );

					nest.push_back(ct);

				}

				nestline = prev + "\x05\x05\x05"; // ��ԍŌ�̂��P������
			}

			// �����̏邪�P�Ɏ��܂��Ă���ꍇ�A���̂悤�Ȍ`�B\x05���Q�ȏ゠��͂��B
			// �O���̊���Ȃ��́u�o��V�i���I�v�̃V�O�i���ŁA=\d���o��V�i���I�̔ԍ�
			else if ( OnigMatch(nestline, "^(\x01.*=\x25\\d)?(.*?)\x20\x1b\x6b(.*?)\x05\x05\x05$", &matches) ) {
				CastleType ct = {NULL};

				// �������͕����l�X�g�̍Ō�̂P�i���C���̂��́j�͑��݂����A����ȊO�͂������Ă���B
				// matches[1]==""�Ƃ������Ƃ́u�������Ă�����̗̂]��̃V�i���I�v�ł��邱�Ƃ��Ӗ�����B
				strcpy( ct.binAppearanceScenario, matches[1].c_str() );

				// �閼�͐�΂ɑ��݂��Ȃ���΂Ȃ�Ȃ�
				// ���p�J�^�J�i���S�p�Ђ炪�Ȃ̗�̂��
				string strFullName = HankakuKatakanaToZentakuKatakana(matches[2].c_str());

				strcpy( ct.szFullName, strFullName.c_str() );

				// �閼�͐�΂ɑ��݂��Ȃ���΂Ȃ�Ȃ�
				strcpy( ct.szRubyYomi, matches[3].c_str() );

				nest.push_back(ct);

				nestline = "";
			}
		}


		// �s���t���܂߁A��͍��킹�����̂��o��
		lstStrFullName->Add( gcnew String(nest[0].szFullName) );

		// �_�~�[�B���ƃR���|�[�l���g�̐������킹�邽�߂����̂���
		lstStrRubyYomi->Add( gcnew String(nest[0].szRubyYomi) );

		// �_�~�[�B���ƃR���|�[�l���g�̐������킹�邽�߂����̂���
		lstStrBornEtc->Add( gcnew String("") );

		// �����邪�P�Ƀp�b�N���ꂽ�̂�(�P��������Ȃ���)�A�S�̃��X�g�ւƒǉ�����B
		vCastleInfoListData.push_back(nest);

	}


	String^ RetsudenAssignManagedData(char *pUnmanagedLine, int ix) {

		string nestline = string(pUnmanagedLine);

		// �P�̃��C���ɕ����̏邪�܂܂�Ă���ꍇ������B�ő�ŃV�i���I���ł���U���܂܂��B
		Matches matches;

		vector<CastleTypeRetsuden> nest;

		for ( int i=0; i<6; i++ ) {

			// �����̏邪�P�Ɏ��܂��Ă���ꍇ�A���̂悤�Ȍ`�B\x05���Q�ȏ゠��͂��B
			if ( OnigMatch(nestline, "^(.+)\x05\x05\x05(.*?)\x05\x05\x05$", &matches ) ) {

				string prev = matches[1]; // �O�ɂ������Ă���c��̏�
				string last = matches[2]; // �Ō�ɂ������Ă����(���)
				CastleTypeRetsuden ctr = {NULL};


				// �O���̊���Ȃ��́u�o��V�i���I�v�̃V�O�i���ŁA=\d���o��V�i���I�̔ԍ�
				if ( OnigMatch(last, "^(\x01.*=\x25\\d)?(.*?)$", &matches) ) {

					string szBufCurrent = HankakuKatakanaToZentakuKatakana(matches[2].c_str());

					// �c��́A���p�J�^�J�i��S�p�Ђ炪�Ȃ�
					char szBufZenHiragana[256] = "";
					hankana2zenhira((unsigned char *)szBufCurrent.c_str(), (unsigned char *)szBufZenHiragana);
					szBufZenHiragana[255] = NULL; // ����ȏC�����Ă��\�����Ȃ��悤�O�̂���

					string strZenHiragana = string(szBufZenHiragana);

					// ���s�I�Ӗ������̂��̂����ȋL���ɒu��������B�������̂��߁B
					for (int i=0; OnigMatch(strZenHiragana, "^(.*?)\x0A(.+)", &matches) && i<(int)strZenHiragana.size(); i++ ) { 
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

					strcpy(ctr.szRetsuden, strZenHiragana.c_str());
					nest.push_back(ctr);
				}

				nestline = prev + "\x05\x05\x05"; // ��ԍŌ�̂��P������
			}
			// �����̏邪�P�Ɏ��܂��Ă���ꍇ�A���̂悤�Ȍ`�B\x05���Q�ȏ゠��͂��B
			// �O���̊���Ȃ��́u�o��V�i���I�v�̃V�O�i���ŁA=\d���o��V�i���I�̔ԍ�
			else if ( OnigMatch(nestline, "^(\x01.*=\x25\\d)?(.*?)\x05\x05\x05$", &matches) ) {

				CastleTypeRetsuden ctr = {NULL};
				string szBufCurrent = HankakuKatakanaToZentakuKatakana(matches[2].c_str());

				// �c��́A���p�J�^�J�i��S�p�Ђ炪�Ȃ�
				char szBufZenHiragana[256] = "";
				hankana2zenhira((unsigned char *)szBufCurrent.c_str(), (unsigned char *)szBufZenHiragana);
				szBufZenHiragana[255] = NULL; // ����ȏC�����Ă��\�����Ȃ��悤�O�̂���

				string strZenHiragana = string(szBufZenHiragana);

				// ���s�I�Ӗ������̂��̂����ȋL���ɒu��������B�������̂��߁B
				for (int i=0; OnigMatch(strZenHiragana, "^(.*?)\x0A(.+)", &matches) && i<(int)strZenHiragana.size(); i++ ) { 
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

				strcpy(ctr.szRetsuden, strZenHiragana.c_str());
				nest.push_back(ctr);

				nestline = "";

			}
		}

		// �����邪�P�Ƀp�b�N���ꂽ�̂�(�P��������Ȃ���)�A�S�̃��X�g�ւƒǉ�����B
		vCastleRetsudenInfoListData.push_back(nest);

		return gcnew String(nest[0].szRetsuden);
	}


	// ��`�n��Unmanaged��Managed�ւƒ��o�B
	String^ RetsudenAssignManagedDataSub(char *pUnmanagedLine) {

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



	// sfile*.dat���z��ւƊi�[�B
	int CastleData_Import() {
		if (!System::IO::File::Exists(gcnew String("sfile1.dat"))) {
			System::Windows::Forms::MessageBox::Show("sfile*.dat��ǂݍ��߂܂���", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
		}

		int iRetsudenNo = -1;

		vBufCastleDecodedData.resize(5);

		for (int ifile = 0; ifile < (int)vBufCastleDecodedData.size(); ifile++) {
			char szFileName[MAX_PATH] = "";
			sprintf(szFileName, "sfile%d.dat", ifile + 1);
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
			vBufCastleDecodedData[ifile] = vdata;
		}

		// �f�R�[�h�����f�[�^��(�����I)�t�@�C���ł܂킵�Ă����B0�Ԃ͑Ώۂł͂Ȃ��B��f�[�^��1�Ԃ���B
		int ivBufDecodedDataSize = vBufCastleDecodedData.size();

		// 18�Ԗڂ���X�^�[�g���Ă���!! ����!!
		for (int ifile = 0; ifile < (int)vBufCastleDecodedData.size(); ifile++) {

			// ���傤�� vSplittedData[0]=�P�Ԗڂ̗v�f�̃f�[�^��AvSplittedData[1]=�Q�Ԗڂ̗v�f�̃f�[�^��A�݂����Ȋ���
			vector<vector<byte>> vSplittedData;

			ls11_SplitData( vBufCastleDecodedData[ifile], &vSplittedData );

			int ivSplittedDataSize = vSplittedData.size();

			// �e�v�f���Ƃɥ�� 
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

						String^ strRetsuden = RetsudenAssignManagedData( bytedata, ielem/2 ); // �}�l�[�W�h�f�[�^�ւƑ��
						lstStrRetsuden->Add( strRetsuden );

						delete bytedata;
					}

				}
			}
		}

		// ���v��`����Ԃ��B
		return iRetsudenNo+1;
	}

	std::string GetOutputFormatNormalizeData(std::string unmanagedData) {

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

	// �Ƃ���P�l�̏�f�[�^���t�@�C���������
	BOOL GetOneCastleImportData(String^ managedFilePath, int iSelectedIndex) {
		return FALSE;
	}

	BOOL AllSaveToMessageN6P() {
		vBufCastleDecodedData.clear(); // ��[�S����
									  // �ƕ��`�S�Ăɂ��ĥ��
		int iFileCnt = (int)ceil((double)lstStrFullName->Count / 50.0); // ���z�t�@�C��(��������)�����J�E���g���B�P������50�Ȃ̂ŁB

																   // ���ꂼ��̉ƕ�ԍ��̊J�n�ԍ��𑗂�Ȃ���A���z�t�@�C���t�@�C�����J��Ԃ��BvBufKahouDecodedData �ɕt���������Ă䂭�B
		for (int f = 0; f<iFileCnt; f++) {
			MakeSplittedDataToJoindData(f * 50);
		}

		int result = 0;
		for (int ifile = 0; ifile < (int)vBufCastleDecodedData.size(); ifile++) {
			char szFileName[MAX_PATH] = "";
			sprintf(szFileName, "ifile%d.dat", ifile + 1);
			FILE *fp = fopen(szFileName, "wb");
			if (!fp) {
				result = -1;
			}
			byte tmpBuf[20000];
			int iDataSize = vBufCastleDecodedData[ifile].size();
			memcpy(tmpBuf, vBufCastleDecodedData[ifile].data(), iDataSize);
			fwrite(tmpBuf, iDataSize, 1, fp);
			fclose(fp);
		}

		if (result == 0) {
			System::Windows::Forms::MessageBox::Show(gcnew String(szCastleTargetFileName) + "�ւƏo�͂��܂����B", "����", MessageBoxButtons::OK, ::MessageBoxIcon::Information);
			return TRUE;
		}
		else {
			System::Windows::Forms::MessageBox::Show("�\�����ʃG���[���������܂����B", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
			return FALSE;
		}
	}

	// �P�̉��z�t�@�C��(�������ꂽ���̑���)����������ɍ��o���B
	void MakeSplittedDataToJoindData(int iStartIndex ) {
		vector<vector<byte>> vSplittedData; // 50 ��l����������ł䂭�f�[�^
		vector<byte> vDstJoinedData; // 50 ��l����������ł䂭�f�[�^

									 // 50�l��
		for (int i = iStartIndex; (i < iStartIndex + 50) && (i < lstStrFullName->Count); i++) {
			vSplittedData.push_back(GetNameLineData(i));
			vSplittedData.push_back(GetRetsudenLineData(i));
		}

		// �����������̂����ւƖ߂��B���P�t�@�C������������
		ls11_JoinData(vSplittedData, &vDstJoinedData);

		// �擪�Ƀf�[�^�������S�̂̃A�h���X�𑀍삷��B
		int iDataByteSize = vDstJoinedData.size();
		int iAllDataByteSize = iDataByteSize + 2;
		byte tmpBuf[20000] = { 0 };
		memcpy(tmpBuf + 2, vDstJoinedData.data(), iDataByteSize);
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
		vBufCastleDecodedData.push_back(vNewJoindData);
	}

	// �Ƃ���P�l���̏�̖��O���N�n�̃f�[�^
	vector<byte> GetNameLineData(int iTargetIndex) {

		std::string unmanagedData = "";
		int packcnt = vCastleInfoListData[iTargetIndex].size();

		// �p�b�N����Ă��镪�A�S����������
		for ( int j=0; j <packcnt; j++ ) {
			unmanagedData = 
				string("") + vCastleInfoListData[iTargetIndex][j].binAppearanceScenario +
				vCastleInfoListData[iTargetIndex][j].szFullName + "\x20\x1b\x6b" +
				vCastleInfoListData[iTargetIndex][j].szRubyYomi + "\x05\x05\x05" +
				unmanagedData;
		}

		char *psz = (char *)unmanagedData.c_str();

		// ��C��char[]��vector<byte>�ւƃR�s�[
		vector<byte> line( &psz[0], &psz[strlen(psz)] );

		return line;
	}

	// �Ƃ���1�l���̏�̗�`�f�[�^
	vector<byte> GetRetsudenLineData(int iTargetIndex) {
		std::string unmanagedData = "";
		int packcnt = vCastleRetsudenInfoListData[iTargetIndex].size();

		// �p�b�N����Ă��镪�A�S����������
		for ( int j=0; j <packcnt; j++ ) {
			unmanagedData = 
				string("") + vCastleInfoListData[iTargetIndex][j].binAppearanceScenario +
				GetOutputFormatNormalizeData( vCastleRetsudenInfoListData[iTargetIndex][j].szRetsuden ) + "\x05\x05\x05" +
				unmanagedData;
		}

		char *psz = (char *)unmanagedData.c_str();

		// ��C��char[]��vector<byte>�ւƃR�s�[
		vector<byte> line( &psz[0], &psz[strlen(psz)] );

		return line;

	}

};