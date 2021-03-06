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

// ちょうど vBufBushouDecodedData[0]=１番目の分割ファイルのデータ列、vBufBushouDecodedData[1]=２番目の分割ファイルのデータ列、みたいな感じ
vector<vector<byte>> vBufBushouDecodedData;


ref class BushouData {
public:
	List<String^>^ lstStrFullName; // 姓名
	List<String^>^ lstStrRubyYomi; // よみがな
	List<String^>^ lstStrBornEtc;  // 生年
	List<String^>^ lstStrRetsuden; // 列伝

public:
	BushouData() {
		lstStrFullName = gcnew List<String^>();
		lstStrRubyYomi = gcnew List<String^>();
		lstStrBornEtc  = gcnew List<String^>();
		lstStrRetsuden = gcnew List<String^>();

		BushouData_Import();
	}

	//  文字列を置換する
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
		// いじくるとバッファが足りなくなるので、何もしない
		// ★HD専用のパッチ
		return szBufOriginal;

		/*


		std::string szBufCurrent = szBufOriginal;

		Matches matches;

		// この特殊な記号は天翔記において、半角カタカナを全角カタカナにするという意味
		for (int i=0; OnigMatch(szBufCurrent, "^(.*?)\x1b\x4b(.*?)\x1b\x48(.*?)$", &matches) && i<(int)szBufCurrent.size(); i++ ) { 
			// 列伝から要素を抽出する。
			if ( !matches[2].empty() ) {
				char szBufZenKatakana[256] = "";
				hankana2zenkana((unsigned char *)matches[2].c_str(), (unsigned char *)szBufZenKatakana);
				szBufZenKatakana[255] = NULL; // 下手な修正しても暴走しないよう念のため

				szBufCurrent = matches[1] + szBufZenKatakana + matches[3];
			}
		}

		// 最後の閉じる記号がないまま行末を迎える場合がある。
		if ( OnigMatch(szBufCurrent, "^(.*?)\x1b\x4b(.+)$", &matches) ) {

			// 列伝から要素を抽出する。
			if ( !matches[2].empty() ) {
				char szBufZenKatakana[256] = "";
				hankana2zenkana((unsigned char *)matches[2].c_str(), (unsigned char *)szBufZenKatakana);
				szBufZenKatakana[255] = NULL; // 下手な修正しても暴走しないよう念のため

				szBufCurrent = matches[1] + szBufZenKatakana;
			}
		}

		return szBufCurrent;

		*/
	}

	// 名前系のUnmanaged→Managedへと抽出。
	void NameAssignManagedData(char *pUnmanagedLine) {
		string line = string(pUnmanagedLine);
		Matches matches;
		if ( OnigMatch(line, "^(.*?) \x1b\x6b(.*?)\x1b\x48 (.*?)\x05\x05\x05$", &matches) ) {

			// 長曽我部と香宗我部の特別処理
			matches[1] = StrReplace(matches[1], "\xEC\x8C\xEC\x8D\xEC\x8E", "長宗我部" );
			matches[1] = StrReplace(matches[1], "\xEC\x8F\xEC\x8D\xEC\x8E", "香宗我部" );

			// 半角⇒全角
			char szBufZenKatakana[256] = "";
			hankana2zenkana((unsigned char *)matches[1].c_str(), (unsigned char *)szBufZenKatakana);
			szBufZenKatakana[255] = NULL; // 下手な修正しても暴走しないよう念のため

			String^ strFullName = gcnew String(szBufZenKatakana);
			lstStrFullName->Add( strFullName );

			String^ strRubyYomi = gcnew String(matches[2].c_str());
			lstStrRubyYomi->Add( strRubyYomi );

			String^ strBornEtc  = gcnew String(matches[3].c_str());
			lstStrBornEtc->Add( strBornEtc );
		}
	}

	// 上のAdd版ではなくて、上書き版｡｡｡ ひどいソースになっておるorz
	BOOL NameOverrideManagedData(char *pUnmanagedLine, int iSelectedIndex) {
		string line = string(pUnmanagedLine);
		Matches matches;
		if ( OnigMatch(line, "^(.*?) \x1b\x6b(.*?)\x1b\x48 (.*?)\x05\x05\x05(.*?)\x05\x05\x05$", &matches) ) {
			// 長曽我部と香宗我部の特別処理
			matches[1] = StrReplace(matches[1], "\xEC\x8C\xEC\x8D\xEC\x8E", "長宗我部" );
			matches[1] = StrReplace(matches[1], "\xEC\x8F\xEC\x8D\xEC\x8E", "香宗我部" );

			// 選択しているところに上書き
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

	// 列伝系のUnmanaged→Managedへと抽出。
	String^ RetsudenAssignManagedData(char *pUnmanagedLine) {

		string szBufCurrent = string(pUnmanagedLine);

		szBufCurrent = HankakuKatakanaToZentakuKatakana(szBufCurrent);

		Matches matches;

		// 最後の終了記号以降は消す。
		if ( OnigMatch(szBufCurrent, "^(.*?)\x05\x05\x05$", &matches) ) {

			if ( !matches[1].empty() ) {
				// 残りは、半角カタカナを全角ひらがなに
				char szBufZenHiragana[256] = "";
				hankana2zenhira((unsigned char *)matches[1].c_str(), (unsigned char *)szBufZenHiragana);
				szBufZenHiragana[255] = NULL; // 下手な修正しても暴走しないよう念のため

				string strZenHiragana = string(szBufZenHiragana);

				// 改行的意味合いのものを特殊な記号に置き換える。高速化のため。
				for (int i=0; OnigMatch(strZenHiragana, "^(.*?)\n(.+)", &matches) && i<(int)strZenHiragana.size(); i++ ) { 
					// 列伝から要素を抽出する。
					if ( !matches[1].empty() ) {
						strZenHiragana = matches[1] + "<r><n>" + matches[2];
					}
				}

				// 特殊な記号を、列伝エディタ上での見た目の改行へと変換。
				for (int i=0; OnigMatch(strZenHiragana, "^(.*?)<r><n>(.+)", &matches) && i<(int)strZenHiragana.size(); i++ ) { 
					// 列伝から要素を抽出する。
					if ( !matches[1].empty() ) {
						strZenHiragana = matches[1] + "\r\n" + matches[2];
					}
				}
				return gcnew String(strZenHiragana.c_str());
			}
		}

		return gcnew String("");
	}



	// bfile*.dat→配列へと格納。
	int BushouData_Import() {
		if (!System::IO::File::Exists(gcnew String("bfile1.dat"))) {
			System::Windows::Forms::MessageBox::Show("bfile*.datを読み込めません", "エラー", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
		}

		int iRetsudenNo = -1;

		vBufBushouDecodedData.resize(26);

		for (int ifile = 0; ifile < (int)vBufBushouDecodedData.size(); ifile++) {
			char szFileName[MAX_PATH] = "";
			sprintf(szFileName, "bfile%d.dat", ifile + 1);
			FILE *fp = fopen(szFileName, "rb");

			// ファイルサイズ
			fpos_t fsize = 0;
			fpos_t fsizeb = fseek(fp, 0, SEEK_END);
			fgetpos(fp, &fsize);

			// 元へと戻す
			rewind(fp);

			byte buf[20000]; // 高速化のため、newしない。

							 // コピー
			fread(buf, (size_t)fsize, 1, fp);

			// 95版と違って、先頭に「64 00」というように全部で100個データがあるという情報が付いている。
			WORD *pDataAddress = (WORD *)buf;
			WORD iDataCount = pDataAddress[0]; // 先頭はデータの個数
			for (int i = 1; i <= iDataCount; i++) {
				pDataAddress[i] = pDataAddress[i] - 2; // 全部アドレスから２を引く
			}

			vector<byte> vdata(&buf[2], &buf[fsize]); // 最初のWORD１つ分は95版には無いカウント数なのでコピーしない。
			vBufBushouDecodedData[ifile] = vdata;
		}

		// デコードしたデータの(内部的)ファイルでまわしていく。0番は対象ではない。武将データは1番から。
		int ivBufDecodedDataSize = vBufBushouDecodedData.size();

		// 1番目からスタートしている!! 注意!!
		for ( int ifile = 0; ifile < (int)ivBufDecodedDataSize ; ifile++ ) {

			// ちょうど vSplittedData[0]=１番目の要素のデータ列、vSplittedData[1]=２番目の要素のデータ列、みたいな感じ
			vector<vector<byte>> vSplittedData;

			ls11_SplitData( vBufBushouDecodedData[ifile], &vSplittedData );

			int ivSplittedDataSize = vSplittedData.size();

			// 各要素ごとに･･･ まず名前、読み仮名 生没年の類
			for ( int ielem=0; ielem < (int)ivSplittedDataSize; ielem++ ) {
				if ( ielem%2==0 ) { iRetsudenNo++; }

				if ( iRetsudenNo < 0 ) { iRetsudenNo = 0; }; // 最悪のための番兵

				// 偶数が読み仮名とか
				if ( ielem%2==0 ) {

					int datasize = vSplittedData[ielem].size();
					if ( datasize > 0 ) {

						char *bytedata = new char[datasize+1];
						memcpy( bytedata, &vSplittedData[ielem][0], datasize );
						bytedata[datasize] = NULL; // 番兵

						NameAssignManagedData( bytedata ); // マネージドデータへと代入

						delete bytedata;
					}


				// 奇数は列伝。
				} else { 

					string szBufCurrent;

					int datasize = vSplittedData[ielem].size();
					if ( datasize > 0 ) {

						char *bytedata = new char[datasize+1];
						memcpy( bytedata, &vSplittedData[ielem][0], datasize );
						bytedata[datasize] = NULL; // 番兵
						szBufCurrent = string(bytedata); 

						String^ strRetsuden = RetsudenAssignManagedData( bytedata ); // マネージドデータへと代入
						lstStrRetsuden->Add( strRetsuden );

						delete bytedata;
					}

				}
			}
		}

		// 合計列伝数を返す。
		return iRetsudenNo+1;
	}

	// とある１人の武将データの出力用バイナリデータを得る
	std::string GetOneBushouExportData(int iSelectedIndex) {
		String^ managedData = String::Format("{0} \x1b\x6b{1}\x1b\x48 {2}\x05\x05\x05{3}\x05\x05\x05", lstStrFullName[iSelectedIndex], lstStrRubyYomi[iSelectedIndex], lstStrBornEtc[iSelectedIndex], lstStrRetsuden[iSelectedIndex]);

		return GetOutputFormatNormalizeData(managedData);

	}

	std::string GetOutputFormatNormalizeData(String^ managedData) {
		// .NETタイプのString^ → マルチバイトのC++型std::stringへ
		std::string unmanagedData = DotNetStringToMultiByte(managedData);

		Matches matches;
		// 特殊な記号を、列伝エディタ上での見た目の改行へと変換。
		for (int i=0; OnigMatch(unmanagedData, "^(.*?)\r\n(.+)", &matches) && i<(int)unmanagedData.size(); i++ ) { 
			// 列伝から要素を抽出する。
			if ( !matches[1].empty() ) {
				unmanagedData = matches[1] + "\x0A" + matches[2];
			}
		}

		return unmanagedData;
	}

	// とある１人の武将データをファイルから入力
	BOOL GetOneBushouImportData(String^ managedFilePath, int iSelectedIndex) {
		std::string unmanagedFilePath = DotNetStringToMultiByte(managedFilePath);

		FILE *fp = fopen( unmanagedFilePath.c_str(), "rb");
		if ( fp ) {
			char pUnmanagedLine[512] = "";
			fread( pUnmanagedLine, sizeof(pUnmanagedLine)-1, 1, fp); // 一気に読み込み
			fclose(fp);


			string line = string(pUnmanagedLine);
			Matches matches;
			if ( OnigMatch(line, "^(.*?) \x1b\x6b(.*?)\x1b\x48 (.*?)\x05\x05\x05(.*?\x05\x05\x05$)", &matches) ) {

				BOOL ret1 = NameOverrideManagedData(pUnmanagedLine, iSelectedIndex);

				if (!ret1) {
					System::Windows::Forms::MessageBox::Show("データの取込に失敗しました。", "エラー", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
					return FALSE;
				}

				// 列伝部分だけ渡す
				String^ strRetsuden = RetsudenAssignManagedData((char *)matches[4].c_str());

				lstStrRetsuden[iSelectedIndex] = strRetsuden;

				System::Windows::Forms::MessageBox::Show("データを取り込みました。。", "完了", MessageBoxButtons::OK, ::MessageBoxIcon::Information);
				return TRUE;
			}

		} else {
			System::Windows::Forms::MessageBox::Show("ファイルのオープンに失敗しました。", "エラー", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
			return FALSE;
		}

		return FALSE;
	}

	BOOL AllSaveToBFileN6P() {
		vBufBushouDecodedData.clear(); // 一端全消去

		// 武将列伝全てについて･･･
		int iFileCnt = (int)ceil((double)lstStrFullName->Count / 50.0); // 仮想ファイル(メモリ上)を作るカウント数。１つあたり50個なので。

		// それぞれの武将番号の開始番号を送りながら、仮想ファイルファイル分繰り返す。vBufBushouDecodedData に付け加えられてゆく。
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
			System::Windows::Forms::MessageBox::Show( gcnew String(szBushouTargetFileName) + "へと出力しました。", "完了", MessageBoxButtons::OK, ::MessageBoxIcon::Information);
			return TRUE;
		} else {
			System::Windows::Forms::MessageBox::Show( "予期せぬエラーが発生しました。", "エラー", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
			return FALSE;
		}
	}

	// １つの仮想ファイル(分割されたもの相当)をメモリ上に作り出す。
	void MakeSplittedDataToJoindData(int iStartIndex) {
		vector<vector<byte>> vSplittedData; // 50 二人分足し込んでゆくデータ
		vector<byte> vDstJoinedData; // 50 二人分足し込んでゆくデータ

		// 50人分
		for ( int i=iStartIndex; (i < iStartIndex + 50) && (i < lstStrFullName->Count); i++ ) {
			vSplittedData.push_back( GetNameLineData(i) );
			vSplittedData.push_back( GetRetsudenLineData(i) );
		}


		// 分解したものを元へと戻す。→１ファイル相当が完成
		ls11_JoinData(vSplittedData, &vDstJoinedData);

		// 先頭にデータ個数をつけつつ全体のアドレスを操作する。
		int iDataByteSize = vDstJoinedData.size();
		int iAllDataByteSize = iDataByteSize + 2;
		byte tmpBuf[20000] = { 0 };
		memcpy(tmpBuf +2, vDstJoinedData.data(), iDataByteSize);
		int iAddress = (int)tmpBuf + 2;
		WORD *wAddress = (WORD *)iAddress;

		int iDataSplittedCount = vSplittedData.size(); // 分解されていたデータ個数
		for (int i = 0; i < iDataSplittedCount; i++) {
			wAddress[i] = wAddress[i] + 2; // ２倍と分、先頭にタス予定なので、アドレスをずらしておく
		}
		tmpBuf[0] = (byte)iDataSplittedCount; // 先頭にデータの個数を設定
		vDstJoinedData.clear();

		vector<byte> vNewJoindData(&tmpBuf[0], &tmpBuf[iAllDataByteSize]);
		// それを全体データに加える。
		vBufBushouDecodedData.push_back(vNewJoindData);
	}

	// とある１人分の武将の名前生年系のデータ
	vector<byte> GetNameLineData(int iTargetIndex) {

		String^ format = String::Format("{0} \x1b\x6b{1}\x1b\x48 {2}\x05\x05\x05", lstStrFullName[iTargetIndex], lstStrRubyYomi[iTargetIndex], lstStrBornEtc[iTargetIndex]);

		std::string unmanagedData = GetOutputFormatNormalizeData(format);

		char *psz = (char *)unmanagedData.c_str();

		// 一気にchar[]→vector<byte>へとコピー
		vector<byte> line( &psz[0], &psz[strlen(psz)] );

		return line;
	}

	// とある1人分の武将の列伝でーた
	vector<byte> GetRetsudenLineData(int iTargetIndex) {
		String^ format = String::Format("{0}\x05\x05\x05", lstStrRetsuden[iTargetIndex]);

		std::string unmanagedData = GetOutputFormatNormalizeData(format);

		char *psz = (char *)unmanagedData.c_str();

		// 一気にchar[]→vector<byte>へとコピー
		vector<byte> line( &psz[0], &psz[strlen(psz)] );

		return line;

	}

};