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


char *szCastleTargetFileName = "sfile[1-5].dat"; // ★

// ちょうど vBufCastleDecodedData[0]=１番目の分割ファイルのデータ列、vBufCastleDecodedData[1]=２番目の分割ファイルのデータ列、みたいな感じ
vector<vector<byte>> vBufCastleDecodedData;


struct CastleType {
	char binAppearanceScenario[100]; // 登場シナリオバイナリ
	char szFullName[32];
	char szRubyYomi[32];
};
struct CastleTypeRetsuden {
	char szRetsuden[512];
};


vector<vector<CastleType>> vCastleInfoListData; // 城情報構造体が複数になってネストしたもの
vector<vector<CastleTypeRetsuden>> vCastleRetsudenInfoListData; // 城情報構造体が複数になってネストしたもの

ref class CastleData {
public:
	List<String^>^ lstStrFullName; // 姓名
	List<String^>^ lstStrRubyYomi; // よみがな
	List<String^>^ lstStrRetsuden; // 列伝
	List<String^>^ lstStrBornEtc;  // 列伝の後ろにあるよくわからないデータ

public:
	CastleData() {
		lstStrFullName = gcnew List<String^>();
		lstStrRubyYomi = gcnew List<String^>();
		lstStrBornEtc  = gcnew List<String^>();
		lstStrRetsuden = gcnew List<String^>();

		CastleData_Import();
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
		string nestline = string(pUnmanagedLine);

		// １つのラインに複数の城が含まれている場合がある。最大でシナリオ数である６つが含まれる。
		Matches matches;

		vector<CastleType> nest;

		for ( int i=0; i<6; i++ ) {

			// 複数の城が１つに収まっている場合、このような形。\x05が２つ以上あるはず。
			if ( OnigMatch(nestline, "^(.+)\x05\x05\x05(.*?)\x05\x05\x05$", &matches ) ) {

				string prev = matches[1]; // 前にくっついている残りの城
				string last = matches[2]; // 最後にくっついている城(主眼)
				CastleType ct = {NULL};


				// 前半の奇怪なられつは「登場シナリオ」のシグナルで、=\dが登場シナリオの番号
				if ( OnigMatch(last, "^(\x01.*=\x25\\d)?(.*?)\x20\x1b\x6b(.*?)$", &matches) ) {
					
					// こっちは複数ネストの最後の１つ（メインのもの）は存在せず、それ以外はくっついている。
					// matches[1]==""ということは「くっついているものの余りのシナリオ」であることを意味する。
					strcpy( ct.binAppearanceScenario, matches[1].c_str() );

					// 城名は絶対に存在しなければならない
					// 半角カタカナ→全角ひらがなの例のやつ
					string strFullName = HankakuKatakanaToZentakuKatakana(matches[2].c_str());

					// 城名は絶対に存在しなければならない
					strcpy( ct.szFullName, strFullName.c_str() );

					// 城名は絶対に存在しなければならない
					strcpy( ct.szRubyYomi, matches[3].c_str() );

					nest.push_back(ct);

				}

				nestline = prev + "\x05\x05\x05"; // 一番最後のを１つけずる
			}

			// 複数の城が１つに収まっている場合、このような形。\x05が２つ以上あるはず。
			// 前半の奇怪なられつは「登場シナリオ」のシグナルで、=\dが登場シナリオの番号
			else if ( OnigMatch(nestline, "^(\x01.*=\x25\\d)?(.*?)\x20\x1b\x6b(.*?)\x05\x05\x05$", &matches) ) {
				CastleType ct = {NULL};

				// こっちは複数ネストの最後の１つ（メインのもの）は存在せず、それ以外はくっついている。
				// matches[1]==""ということは「くっついているものの余りのシナリオ」であることを意味する。
				strcpy( ct.binAppearanceScenario, matches[1].c_str() );

				// 城名は絶対に存在しなければならない
				// 半角カタカナ→全角ひらがなの例のやつ
				string strFullName = HankakuKatakanaToZentakuKatakana(matches[2].c_str());

				strcpy( ct.szFullName, strFullName.c_str() );

				// 城名は絶対に存在しなければならない
				strcpy( ct.szRubyYomi, matches[3].c_str() );

				nest.push_back(ct);

				nestline = "";
			}
		}


		// 《複》も含め、城は合わせたものを出す
		lstStrFullName->Add( gcnew String(nest[0].szFullName) );

		// ダミー。他とコンポーネントの数を合わせるためだけのため
		lstStrRubyYomi->Add( gcnew String(nest[0].szRubyYomi) );

		// ダミー。他とコンポーネントの数を合わせるためだけのため
		lstStrBornEtc->Add( gcnew String("") );

		// 複数城が１つにパックされたのを(１つかもしれないが)、全体リストへと追加する。
		vCastleInfoListData.push_back(nest);

	}


	String^ RetsudenAssignManagedData(char *pUnmanagedLine, int ix) {

		string nestline = string(pUnmanagedLine);

		// １つのラインに複数の城が含まれている場合がある。最大でシナリオ数である６つが含まれる。
		Matches matches;

		vector<CastleTypeRetsuden> nest;

		for ( int i=0; i<6; i++ ) {

			// 複数の城が１つに収まっている場合、このような形。\x05が２つ以上あるはず。
			if ( OnigMatch(nestline, "^(.+)\x05\x05\x05(.*?)\x05\x05\x05$", &matches ) ) {

				string prev = matches[1]; // 前にくっついている残りの城
				string last = matches[2]; // 最後にくっついている城(主眼)
				CastleTypeRetsuden ctr = {NULL};


				// 前半の奇怪なられつは「登場シナリオ」のシグナルで、=\dが登場シナリオの番号
				if ( OnigMatch(last, "^(\x01.*=\x25\\d)?(.*?)$", &matches) ) {

					string szBufCurrent = HankakuKatakanaToZentakuKatakana(matches[2].c_str());

					// 残りは、半角カタカナを全角ひらがなに
					char szBufZenHiragana[256] = "";
					hankana2zenhira((unsigned char *)szBufCurrent.c_str(), (unsigned char *)szBufZenHiragana);
					szBufZenHiragana[255] = NULL; // 下手な修正しても暴走しないよう念のため

					string strZenHiragana = string(szBufZenHiragana);

					// 改行的意味合いのものを特殊な記号に置き換える。高速化のため。
					for (int i=0; OnigMatch(strZenHiragana, "^(.*?)\x0A(.+)", &matches) && i<(int)strZenHiragana.size(); i++ ) { 
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

					strcpy(ctr.szRetsuden, strZenHiragana.c_str());
					nest.push_back(ctr);
				}

				nestline = prev + "\x05\x05\x05"; // 一番最後のを１つけずる
			}
			// 複数の城が１つに収まっている場合、このような形。\x05が２つ以上あるはず。
			// 前半の奇怪なられつは「登場シナリオ」のシグナルで、=\dが登場シナリオの番号
			else if ( OnigMatch(nestline, "^(\x01.*=\x25\\d)?(.*?)\x05\x05\x05$", &matches) ) {

				CastleTypeRetsuden ctr = {NULL};
				string szBufCurrent = HankakuKatakanaToZentakuKatakana(matches[2].c_str());

				// 残りは、半角カタカナを全角ひらがなに
				char szBufZenHiragana[256] = "";
				hankana2zenhira((unsigned char *)szBufCurrent.c_str(), (unsigned char *)szBufZenHiragana);
				szBufZenHiragana[255] = NULL; // 下手な修正しても暴走しないよう念のため

				string strZenHiragana = string(szBufZenHiragana);

				// 改行的意味合いのものを特殊な記号に置き換える。高速化のため。
				for (int i=0; OnigMatch(strZenHiragana, "^(.*?)\x0A(.+)", &matches) && i<(int)strZenHiragana.size(); i++ ) { 
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

				strcpy(ctr.szRetsuden, strZenHiragana.c_str());
				nest.push_back(ctr);

				nestline = "";

			}
		}

		// 複数城が１つにパックされたのを(１つかもしれないが)、全体リストへと追加する。
		vCastleRetsudenInfoListData.push_back(nest);

		return gcnew String(nest[0].szRetsuden);
	}


	// 列伝系のUnmanaged→Managedへと抽出。
	String^ RetsudenAssignManagedDataSub(char *pUnmanagedLine) {

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



	// sfile*.dat→配列へと格納。
	int CastleData_Import() {
		if (!System::IO::File::Exists(gcnew String("sfile1.dat"))) {
			System::Windows::Forms::MessageBox::Show("sfile*.datを読み込めません", "エラー", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
		}

		int iRetsudenNo = -1;

		vBufCastleDecodedData.resize(5);

		for (int ifile = 0; ifile < (int)vBufCastleDecodedData.size(); ifile++) {
			char szFileName[MAX_PATH] = "";
			sprintf(szFileName, "sfile%d.dat", ifile + 1);
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
			vBufCastleDecodedData[ifile] = vdata;
		}

		// デコードしたデータの(内部的)ファイルでまわしていく。0番は対象ではない。城データは1番から。
		int ivBufDecodedDataSize = vBufCastleDecodedData.size();

		// 18番目からスタートしている!! 注意!!
		for (int ifile = 0; ifile < (int)vBufCastleDecodedData.size(); ifile++) {

			// ちょうど vSplittedData[0]=１番目の要素のデータ列、vSplittedData[1]=２番目の要素のデータ列、みたいな感じ
			vector<vector<byte>> vSplittedData;

			ls11_SplitData( vBufCastleDecodedData[ifile], &vSplittedData );

			int ivSplittedDataSize = vSplittedData.size();

			// 各要素ごとに･･･ 
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

						String^ strRetsuden = RetsudenAssignManagedData( bytedata, ielem/2 ); // マネージドデータへと代入
						lstStrRetsuden->Add( strRetsuden );

						delete bytedata;
					}

				}
			}
		}

		// 合計列伝数を返す。
		return iRetsudenNo+1;
	}

	std::string GetOutputFormatNormalizeData(std::string unmanagedData) {

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

	// とある１人の城データをファイルから入力
	BOOL GetOneCastleImportData(String^ managedFilePath, int iSelectedIndex) {
		return FALSE;
	}

	BOOL AllSaveToMessageN6P() {
		vBufCastleDecodedData.clear(); // 一端全消去
									  // 家宝列伝全てについて･･･
		int iFileCnt = (int)ceil((double)lstStrFullName->Count / 50.0); // 仮想ファイル(メモリ上)を作るカウント数。１つあたり50個なので。

																   // それぞれの家宝番号の開始番号を送りながら、仮想ファイルファイル分繰り返す。vBufKahouDecodedData に付け加えられてゆく。
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
			System::Windows::Forms::MessageBox::Show(gcnew String(szCastleTargetFileName) + "へと出力しました。", "完了", MessageBoxButtons::OK, ::MessageBoxIcon::Information);
			return TRUE;
		}
		else {
			System::Windows::Forms::MessageBox::Show("予期せぬエラーが発生しました。", "エラー", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
			return FALSE;
		}
	}

	// １つの仮想ファイル(分割されたもの相当)をメモリ上に作り出す。
	void MakeSplittedDataToJoindData(int iStartIndex ) {
		vector<vector<byte>> vSplittedData; // 50 二人分足し込んでゆくデータ
		vector<byte> vDstJoinedData; // 50 二人分足し込んでゆくデータ

									 // 50人分
		for (int i = iStartIndex; (i < iStartIndex + 50) && (i < lstStrFullName->Count); i++) {
			vSplittedData.push_back(GetNameLineData(i));
			vSplittedData.push_back(GetRetsudenLineData(i));
		}

		// 分解したものを元へと戻す。→１ファイル相当が完成
		ls11_JoinData(vSplittedData, &vDstJoinedData);

		// 先頭にデータ個数をつけつつ全体のアドレスを操作する。
		int iDataByteSize = vDstJoinedData.size();
		int iAllDataByteSize = iDataByteSize + 2;
		byte tmpBuf[20000] = { 0 };
		memcpy(tmpBuf + 2, vDstJoinedData.data(), iDataByteSize);
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
		vBufCastleDecodedData.push_back(vNewJoindData);
	}

	// とある１人分の城の名前生年系のデータ
	vector<byte> GetNameLineData(int iTargetIndex) {

		std::string unmanagedData = "";
		int packcnt = vCastleInfoListData[iTargetIndex].size();

		// パックされている分、全部足しこむ
		for ( int j=0; j <packcnt; j++ ) {
			unmanagedData = 
				string("") + vCastleInfoListData[iTargetIndex][j].binAppearanceScenario +
				vCastleInfoListData[iTargetIndex][j].szFullName + "\x20\x1b\x6b" +
				vCastleInfoListData[iTargetIndex][j].szRubyYomi + "\x05\x05\x05" +
				unmanagedData;
		}

		char *psz = (char *)unmanagedData.c_str();

		// 一気にchar[]→vector<byte>へとコピー
		vector<byte> line( &psz[0], &psz[strlen(psz)] );

		return line;
	}

	// とある1人分の城の列伝データ
	vector<byte> GetRetsudenLineData(int iTargetIndex) {
		std::string unmanagedData = "";
		int packcnt = vCastleRetsudenInfoListData[iTargetIndex].size();

		// パックされている分、全部足しこむ
		for ( int j=0; j <packcnt; j++ ) {
			unmanagedData = 
				string("") + vCastleInfoListData[iTargetIndex][j].binAppearanceScenario +
				GetOutputFormatNormalizeData( vCastleRetsudenInfoListData[iTargetIndex][j].szRetsuden ) + "\x05\x05\x05" +
				unmanagedData;
		}

		char *psz = (char *)unmanagedData.c_str();

		// 一気にchar[]→vector<byte>へとコピー
		vector<byte> line( &psz[0], &psz[strlen(psz)] );

		return line;

	}

};