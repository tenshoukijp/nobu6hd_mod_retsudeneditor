#pragma once;

#include "BushouData.h"
#include "KahouData.h"
#include "CastleData.h"

// RetsudenEditor.cpp : メイン プロジェクト ファイルです。
#include <windows.h>
#include <string>

#include "DotNetTestToMultiByte.h"

ref class RetsudenEditorForm : public Form {

//-----------------------------------------------フォームウィンドウ系

	TabControl^ tcRE;	// 全体で１つになってるタブコントロール。TabPage型を追加してく

    static cli::array<String^>^ aryStrFontCandidate = {"天翔 明朝", "ＭＳ 明朝", "ＭＳ ゴシック" }; // フォントリスト

public:
	RetsudenEditorForm() {
		this->AutoScaleMode = ::AutoScaleMode::Dpi;

		// フォームにかかわるものの初期化等
		this->Text = "HD版 列伝エディタ";
		this->Width = 800;
		this->Height = 500;

		SetFormIcon();

		SetTabControl();
	}

	void SetFormIcon() {
		// このプロジェクトのアセンブリのタイプを取得。
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		System::Drawing::Icon^ iconform = (System::Drawing::Icon^)(r->GetObject("icon"));
		this->Icon = iconform;
	}

	// タブコントロールの初期化。間に、各データグリッドビューへのデータ流し込み処理が入り込む
	void SetTabControl() {
		tcRE = gcnew TabControl();
		tcRE->Size = this->ClientSize;
		
		// 武将列伝のタブページ追加
		tpBushou_Init();
		Bushou_SetEventHander();

		// 家宝列伝のタブページ追加
		tpKahou_Init();
		Kahou_SetEventHander();

		// 城列伝のタブページ追加
		tpCastle_Init();
		Castle_SetEventHander();

		tcRE->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::tcRE_SelectedIndexChanged);
		this->Controls->Add(tcRE);
	}

	// タブを切り替えた時
	void tcRE_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
	}


	int GetStringMeasure(String^ str) {
		char szBufMeasure[128];
		wsprintf( szBufMeasure, "%s", str );
		int iByteNum = strlen( szBufMeasure );
		return iByteNum * 8 + 8;
	}

//-----------------------------------------------武将列伝系
private:
	TabPage^ tpBushou;
	BushouData^ bdBushouData;

	TextBox^ tbBushouSearch; // 武将検索
	Button^ btnBushouSearch; // 検索ボタン
	Button^ btnBushouImport; // 取込ボタン
	OpenFileDialog^ ofdBushouImport;
	Button^ btnBushouExport; // 出力ボタン
	Button^ btnBushouAllSave; // 全て保存

	ListBox^ lbBushouList; // 武将リストのコンボボックス
	int iLastBushouListSelected;

	Panel^ plBushouRetsuden;	 // 列伝情報用のパネル
	TextBox^ tbBushouFullName; // 武将姓名のテキストボックス
	TextBox^ tbBushouRubyYomi; // ふりがなのるび
	TextBox^ tbBushouBornEtc;  // 生年などの情報
	TextBox^ tbBushouRetsuden; // 列伝情報


	// フォント
	System::Drawing::Font^ fontBushou;
	ComboBox^ cbFontBushou;	// フォントリスト

	Button^ btnBushouAdds; // 50枠追加
	Button^ btnBushouDels; // 50枠追加


private:

	void tpBushou_Init() {

		tpBushou = gcnew TabPage();

		tpBushou->Text = "武将列伝";
		tpBushou->Size = tcRE->ClientSize;

		// 武将検索
		tbBushouSearch = gcnew TextBox();
		tbBushouSearch->Width = 210;
		tbBushouSearch->Height = 24;
		tbBushouSearch->Left = 10;
		tbBushouSearch->Top = 10;

		// 検索ボタン
		btnBushouSearch = gcnew Button();
		btnBushouSearch->Text = "検索";
		btnBushouSearch->Width = 90;
		btnBushouSearch->Height = tbBushouSearch->Height;
		btnBushouSearch->Left = tbBushouSearch->Right + 1;
		btnBushouSearch->Top = tbBushouSearch->Top;

		// 取込ボタン
		btnBushouImport = gcnew Button();
		btnBushouImport->Text = "単独取込";
		btnBushouImport->Width = 105;
		btnBushouImport->Height = tbBushouSearch->Height;
		btnBushouImport->Left = btnBushouSearch->Right + 45;
		btnBushouImport->Top = tbBushouSearch->Top;

		// 出力ボタン
		btnBushouExport = gcnew Button();
		btnBushouExport->Text = "単独出力";
		btnBushouExport->Width = 105;
		btnBushouExport->Height = tbBushouSearch->Height;
		btnBushouExport->Left = btnBushouImport->Right;
		btnBushouExport->Top = tbBushouSearch->Top;

		// 全て保存
		btnBushouAllSave = gcnew Button();
		btnBushouAllSave->Text = "全て保存";
		btnBushouAllSave->Width = 105;
		btnBushouAllSave->Height = tbBushouSearch->Height;
		btnBushouAllSave->Left = tpBushou->Right - (btnBushouAllSave->Width+30);
		btnBushouAllSave->Top = tbBushouSearch->Top;

		// 武将知ると
		lbBushouList = gcnew ListBox();
		lbBushouList->Width = tbBushouSearch->Width;
		lbBushouList->Left = tbBushouSearch->Left;
		lbBushouList->Top = tbBushouSearch->Bottom + 15;
		lbBushouList->Height = 300;

		// 列伝情報用のパネル
		plBushouRetsuden = gcnew Panel();
		plBushouRetsuden->Top = lbBushouList->Top;
		Bushou_SetPanelBackImg();
		plBushouRetsuden->Left = lbBushouList->Right+10;
		plBushouRetsuden->Width = 600;
		plBushouRetsuden->Height = 350;

		tbBushouFullName = gcnew TextBox();
		tbBushouFullName->Top = 15;
		tbBushouFullName->Left = 15;
		tbBushouFullName->Width = 160;
		tbBushouFullName->MaxLength = 37;
		tbBushouFullName->BackColor = Color::FromArgb(0xC0, 0xD0, 0xF0);

		tbBushouRubyYomi = gcnew TextBox();
		tbBushouRubyYomi->Top = tbBushouFullName->Top;
		tbBushouRubyYomi->Left = tbBushouFullName->Right+10;
		tbBushouRubyYomi->MaxLength = 37;
		tbBushouRubyYomi->Width = 160;
		tbBushouRubyYomi->BackColor = Color::FromArgb(0xC0, 0xD0, 0xF0);

		tbBushouBornEtc  = gcnew TextBox();
		tbBushouBornEtc->Top = tbBushouFullName->Top;
		tbBushouBornEtc->Left = tbBushouRubyYomi->Right+10;
		tbBushouBornEtc->Width = 160;
		tbBushouBornEtc->MaxLength = 37;
		tbBushouBornEtc->BackColor = Color::FromArgb(0xC0, 0xD0, 0xF0);

		tbBushouRetsuden = gcnew TextBox();
		tbBushouRetsuden->Top = tbBushouFullName->Bottom + 12;
		tbBushouRetsuden->Left = tbBushouFullName->Left;
		tbBushouRetsuden->Height = 135;
		tbBushouRetsuden->Width = (int)(312 * 1.5 + 32);
		tbBushouRetsuden->MaxLength = 2 * 27 * 4;
		tbBushouRetsuden->Multiline = true;
		tbBushouRetsuden->WordWrap = false;
		tbBushouRetsuden->BackColor = Color::FromArgb(0xC0, 0xD0, 0xF0);
		

		plBushouRetsuden->Controls->Add(tbBushouFullName);
		plBushouRetsuden->Controls->Add(tbBushouRubyYomi);
		plBushouRetsuden->Controls->Add(tbBushouBornEtc);
		plBushouRetsuden->Controls->Add(tbBushouRetsuden);

		// フォントリスト
		cbFontBushou = gcnew ComboBox();
		cbFontBushou->Width = 150;
		cbFontBushou->Left = lbBushouList->Right + 10;
		cbFontBushou->Top = plBushouRetsuden->Bottom + 15;
		cbFontBushou->Height = tbBushouSearch->Height;
		Bushou_GetValidTenshoukiFontList();

		Bushou_cbFont_SelectedIndexChanged(nullptr, nullptr);

		// 武将枠50人追加
		btnBushouAdds = gcnew Button();
		btnBushouAdds->Text = "50枠追加";
		btnBushouAdds->Width = 70;
		btnBushouAdds->Height = tbBushouSearch->Height;
		btnBushouAdds->Left = tbBushouSearch->Left;
		btnBushouAdds->Top = lbBushouList->Bottom + 10;

		// 武将枠50人削除
		btnBushouDels = gcnew Button();
		btnBushouDels->Text = "50枠削除";
		btnBushouDels->Width = 70;
		btnBushouDels->Height = tbBushouSearch->Height;
		btnBushouDels->Left = btnBushouAdds->Right;
		btnBushouDels->Top = lbBushouList->Bottom + 10;
		btnBushouAdds->Enabled = false;


		// 武将列伝のタブにコンポーネント追加
		tpBushou->Controls->Add(tbBushouSearch);
		tpBushou->Controls->Add(btnBushouSearch);
//		tpBushou->Controls->Add(btnBushouImport);
//		tpBushou->Controls->Add(btnBushouExport);
		tpBushou->Controls->Add(btnBushouAllSave);
		tpBushou->Controls->Add(lbBushouList);
		tpBushou->Controls->Add(plBushouRetsuden);
//		tpBushou->Controls->Add(cbFontBushou);
//		tpBushou->Controls->Add(btnBushouAdds);
//		tpBushou->Controls->Add(btnBushouDels);
		
		// タブをフォームに追加
		tcRE->TabPages->Add(tpBushou);

		// 武将データを作成して、リストに流しこむ
		bdBushouData = gcnew BushouData();
		// リストボックスに足し込み
		for ( int i=0; i<bdBushouData->lstStrFullName->Count; i++ ) {
			String^ strLine = String::Format( "{0:0000} - {1}", i, bdBushouData->lstStrFullName[i]);
			lbBushouList->Items->Add( strLine );
		}

		// 最初のものを選択しておく
		lbBushouList->SelectedIndex = 0;
		iLastBushouListSelected = 0;

		// 武将データ→テキストボックス
		Bushou_BDataToTextBox();
		Bushou_TextBoxWidthUpdate();
	}

	// パネルの背景画像設定
	void Bushou_SetPanelBackImg() {
		// 不要
		return;

		// このプロジェクトのアセンブリのタイプを取得。
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		// パネルの背景
		plBushouRetsuden->BackgroundImage = (System::Drawing::Image^)(r->GetObject("retsuden_back"));
		// 背景画像の大きさにパネルの大きさを合わせる
		plBushouRetsuden->Size = plBushouRetsuden->BackgroundImage->Size;

	}

	// 各種ＧＵＩのイベントハンドラ設定
	void Bushou_SetEventHander() {
		btnBushouSearch->Click += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_btnSearch_Click);
		btnBushouImport->Click += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_btnImport_Click);
		btnBushouExport->Click += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_btnExport_Click);
		btnBushouAllSave->Click += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_btnAllSave_Click);
		tbBushouSearch->KeyDown += gcnew KeyEventHandler(this, &RetsudenEditorForm::Bushou_tbSearch_KeyDown);
		lbBushouList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_lbBushouList_SelectedIndexChanged);
		tbBushouFullName->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Bushou_tbFullName_TextChanged);
		tbBushouRubyYomi->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Bushou_tbRubyYomi_TextChanged);
		tbBushouBornEtc->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Bushou_tbBornEtc_TextChanged);
		tbBushouRetsuden->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Bushou_tbRetsuden_TextChanged);
		tbBushouFullName->Leave += gcnew EventHandler( this, &RetsudenEditorForm::Bushou_textBox_Leave);
		tbBushouRubyYomi->Leave += gcnew EventHandler( this, &RetsudenEditorForm::Bushou_textBox_Leave);
		tbBushouBornEtc->Leave += gcnew EventHandler( this, &RetsudenEditorForm::Bushou_textBox_Leave);
		tbBushouRetsuden->Leave += gcnew EventHandler( this, &RetsudenEditorForm::Bushou_textBox_Leave);
		cbFontBushou->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_cbFont_SelectedIndexChanged);
		btnBushouAdds->Click += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_btnAddBushous_Click);
		btnBushouDels->Click += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_btnDelBushous_Click);
	}

	// 武将検索ボタンを押した時
	void Bushou_btnSearch_Click(Object^ sender, EventArgs^ e) {
		if ( tbBushouSearch->Text == "" ) { return; } // テキストエリアが空っぽなら何もしない。

		int iStart = GetBushouCBSelectedIndex() + 1; // 現在選択しているもののから次の候補

		Regex^ regex = gcnew Regex(tbBushouSearch->Text);

		bool isExist = false;
		// iStart以降を全部サーチして、
		for ( int i=iStart; i<lbBushouList->Items->Count; i++) {
			Match^ match = regex->Match( (String^)lbBushouList->Items[i] );

			if ( match->Success ) {
				lbBushouList->SelectedIndex = i;
				isExist = true;
				break;
			}
		}

		if ( !isExist ) {
			for ( int i=0; i<iStart; i++) {
				Match^ match = regex->Match( (String^)lbBushouList->Items[i] );

				if ( match->Success ) {
					lbBushouList->SelectedIndex = i;
					break;
				}
			}
		}
	}

	// 武将検索テキストボックスでキーを押した時
	void Bushou_tbSearch_KeyDown(Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Return ) {
			Bushou_btnSearch_Click(sender, e);
		}
		if (e->KeyCode == Keys::F3 ) {
			Bushou_btnSearch_Click(sender, e);
		}
	}

	// 武将出力ボタンを押した時
	void Bushou_btnExport_Click(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		std::string unmanagedData = bdBushouData->GetOneBushouExportData(iSelectedIndex);

		std::string filepath = DotNetStringToMultiByte(bdBushouData->lstStrFullName[iSelectedIndex]);
		if ( filepath == "" ) { filepath = "ななし"; }
		filepath += ".n6b";

		FILE *fp = fopen(filepath.c_str(), "wb");
		if ( fp ) {
			fwrite( unmanagedData.c_str(), unmanagedData.size(), 1, fp);
			fclose(fp);
			System::Windows::Forms::MessageBox::Show("データを出力しました。", "完了", ::MessageBoxButtons::OK, ::MessageBoxIcon::Information);
		} else {
			System::Windows::Forms::MessageBox::Show("データの出力に失敗しました。", "エラー", MessageBoxButtons::OK, ::MessageBoxIcon::Error);

		}
	}


	// 武将入力ボタンを押した時
	void Bushou_btnImport_Click(Object^ sender, EventArgs^ e) {

		ofdBushouImport = gcnew OpenFileDialog();

		ofdBushouImport->InitialDirectory = "."; // ツールと同じディレクトリがデフォルト

		ofdBushouImport->Filter = "天翔記武将列伝ファイル(*.n6b)|*.n6b";

		ofdBushouImport->FilterIndex = 1;

		ofdBushouImport->Title = "とある武将の列伝ファイルを選択してください";

		//ダイアログボックスを閉じる前に現在のディレクトリを復元するようにする
		ofdBushouImport->RestoreDirectory = true;

		//ダイアログを表示する
		if (ofdBushouImport->ShowDialog() == ::DialogResult::OK) {
			//OKボタンがクリックされたとき
			if ( ofdBushouImport->FileName ) {
				BOOL success = bdBushouData->GetOneBushouImportData(ofdBushouImport->FileName, GetBushouCBSelectedIndex());

				//　成否にかかわらず更新
				Bushou_BDataToTextBox();
			}
		}
	}

	// 全て保存ボタンを押した時
	void Bushou_btnAllSave_Click(Object^ sender, EventArgs^ e) {
		bdBushouData->AllSaveToBFileN6P();
	}

	// フォントリストの選択項目が変更された時
	void Bushou_cbFont_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		// フォントの設定
		fontBushou = gcnew System::Drawing::Font((String^)cbFontBushou->Items[cbFontBushou->SelectedIndex], 12, FontStyle::Regular);
		tbBushouFullName->Font = fontBushou;
		tbBushouRubyYomi->Font = fontBushou;
		tbBushouBornEtc->Font  = fontBushou;
		tbBushouRetsuden->Font = fontBushou;
	}

	// インストールされているフォントにしたがって、フォントリストボックスに追加
	void Bushou_GetValidTenshoukiFontList() {

		//InstalledFontCollectionオブジェクトの取得
		System::Drawing::Text::InstalledFontCollection^ ifc = gcnew System::Drawing::Text::InstalledFontCollection();

		//インストールされているすべてのフォントファミリアを取得
		cli::array<FontFamily^>^ ffs = ifc->Families;

		//ここでは候補となりえるフォント名のみ
		for ( int i=0; i<aryStrFontCandidate->Length; i++) {

			// フォントリスト
			for each (FontFamily^ ff in ffs) {

				// 候補フォントがあった。
				if (ff->Name == aryStrFontCandidate[i]) {
					// コンボボックスに追加
					cbFontBushou->Items->Add( ff->Name );
				}
			}
		}

		// １つ以上フォントがあったら、最初のものを選択しておく。
		if ( cbFontBushou->Items->Count > 0 ) {
			cbFontBushou->SelectedIndex = 0;
		}
	}

	int GetBushouCBSelectedIndex() {
		int iSelectedIndex = lbBushouList->SelectedIndex;
		if ( 0 <= iSelectedIndex && iSelectedIndex < lbBushouList->Items->Count) {
			return iSelectedIndex;
		} else {
			return 0;
		}
	}
	void ModifyCBBushouSelectedIndex() {
		// 焦点を見失っているようなら、最後に選択していたものを宛がう。
		int iSelectIndex = GetBushouCBSelectedIndex();
		if ( iSelectIndex == 0xFFFFFFFF ) {
			lbBushouList->SelectedIndex = iLastBushouListSelected;
		} else {
			iLastBushouListSelected = iSelectIndex;
		}
	}


	// 武将リストを選択変更すると、画像の上の列伝各種の値が入れ替わる
	void Bushou_lbBushouList_SelectedIndexChanged(Object^ sender, EventArgs^ e ) {
		ModifyCBBushouSelectedIndex();
		// 新たなindexのデータを取得。
		Bushou_BDataToTextBox();

		ModifyCBBushouSelectedIndex();
	}


	// 武将データ→テキストボックスへ転写
	void Bushou_BDataToTextBox() {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		tbBushouFullName->Text = bdBushouData->lstStrFullName[iSelectedIndex];
		tbBushouRubyYomi->Text = bdBushouData->lstStrRubyYomi[iSelectedIndex];
		tbBushouBornEtc->Text  = bdBushouData->lstStrBornEtc[iSelectedIndex];
		tbBushouRetsuden->Text = bdBushouData->lstStrRetsuden[iSelectedIndex];
	}

	// テキストボックスの位置や幅を中身のテキスト量に従って変更
	void Bushou_TextBoxWidthUpdate() {
		// tbBushouFullName->Width = GetStringMeasure(tbBushouFullName->Text);

		// tbBushouRubyYomi->Left = tbBushouFullName->Right + 8;
		// tbBushouRubyYomi->Width = GetStringMeasure(tbBushouRubyYomi->Text);
		
		// tbBushouBornEtc->Left = tbBushouRubyYomi->Right + 8;
		// tbBushouBornEtc->Width = GetStringMeasure(tbBushouBornEtc->Text);
	}

	// テキストボックスの、「武将姓名」が変更された。
	void Bushou_tbFullName_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		bdBushouData->lstStrFullName[iSelectedIndex] = tbBushouFullName->Text;
		String^ strLine = String::Format( "{0:0000} - {1}", iSelectedIndex, tbBushouFullName->Text);
		// SelectIndexChangeが反応しないようにする。
		lbBushouList->SelectedIndexChanged -= gcnew EventHandler(this, &RetsudenEditorForm::Bushou_lbBushouList_SelectedIndexChanged);

		// 反応しないようにしたので、値を書き換える。SelectedIndexChangeメソッドが反応しないのでエラーが起きない。

		lbBushouList->Items[iSelectedIndex] = gcnew String(strLine); // 武将リストの名前の方を更新

		Bushou_TextBoxWidthUpdate();

		// 更新したので、反応を戻す。
		// SelectIndexChangeが反応するようにする。
		lbBushouList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_lbBushouList_SelectedIndexChanged);
	}

	// テキストボックスの、「ふりがな」が変更された。
	void Bushou_tbRubyYomi_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		bdBushouData->lstStrRubyYomi[iSelectedIndex] = tbBushouRubyYomi->Text;
		Bushou_TextBoxWidthUpdate();
	}

	// テキストボックスの、「生年等」が変更された。
	void Bushou_tbBornEtc_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		bdBushouData->lstStrBornEtc[iSelectedIndex]  = tbBushouBornEtc->Text;
		Bushou_TextBoxWidthUpdate();
	}

	// テキストボックスの、「列伝」が変更された。
	void Bushou_tbRetsuden_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetBushouCBSelectedIndex();

		bdBushouData->lstStrRetsuden[iSelectedIndex] = tbBushouRetsuden->Text;
	}

	// テキストボックスから去った
	void Bushou_textBox_Leave(Object^ sender, EventArgs^ e ) {
	}


	// 50枠追加ボタン
	void Bushou_btnAddBushous_Click(Object^ sender, EventArgs^ e) {
		int iStart = lbBushouList->Items->Count;
		for ( int i = iStart; i < iStart+50; i++ ) {
			String^ bname = String::Format("登録{0:0000}", i);
			// 先に元データから増やす
			bdBushouData->lstStrFullName->Add( bname );
			bdBushouData->lstStrRubyYomi->Add( "ﾄｳﾛｸ"+i );
			bdBushouData->lstStrBornEtc->Add( "????〜????" );
			bdBushouData->lstStrRetsuden->Add( "未入力" );
			String^ strLine = String::Format( "{0:0000} - {1}", i, bname);
			lbBushouList->Items->Add( strLine );
		}

	}

	// 50枠削除ボタン
	void Bushou_btnDelBushous_Click(Object^ sender, EventArgs^ e) {
		
		int iStart = lbBushouList->Items->Count-1;

		// 選択インデックスが削除予定の所にあったら、ギリギリのところまで退避
		if ( GetBushouCBSelectedIndex() > iStart - 50 ) {
			lbBushouList->SelectedIndex = iStart - 50;
		}

		for ( int i = iStart; i > iStart-50; i-- ) {
			// 先に武将リストからカット
			lbBushouList->Items->RemoveAt(i);

			// 次いで元データを減らす
			bdBushouData->lstStrFullName->RemoveAt( i );
			bdBushouData->lstStrRubyYomi->RemoveAt( i );
			bdBushouData->lstStrBornEtc->RemoveAt( i );
			bdBushouData->lstStrRetsuden->RemoveAt( i );
		}
	}


//-----------------------------------------------家宝列伝系
private:
	TabPage^ tpKahou;
	KahouData^ bdKahouData;

	TextBox^ tbKahouSearch; // 家宝検索
	Button^ btnKahouSearch; // 検索ボタン
	Button^ btnKahouImport; // 取込ボタン
	Button^ btnKahouExport; // 出力ボタン
	Button^ btnKahouAllSave; // 全て保存

	ListBox^ lbKahouList; // 家宝リストのコンボボックス
	int iLastKahouListSelected;

	Panel^ plKahouRetsuden;	 // 列伝情報用のパネル
	TextBox^ tbKahouFullName; // 家宝姓名のテキストボックス
	TextBox^ tbKahouRubyYomi; // ふりがなのるび
	TextBox^ tbKahouBornEtc;  // 生年などの情報
	TextBox^ tbKahouRetsuden; // 列伝情報


	// フォント
	System::Drawing::Font^ fontKahou;
	ComboBox^ cbFontKahou;	// フォントリスト

	Button^ btnKahouAdds; // 50枠追加
	Button^ btnKahouDels; // 50枠追加


private:

	void tpKahou_Init() {

		tpKahou = gcnew TabPage();
		tpKahou->BackColor = Color::Transparent;


		tpKahou->Text = "家宝列伝";
		tpKahou->Size = tcRE->ClientSize;

		// 家宝検索
		tbKahouSearch = gcnew TextBox();
		tbKahouSearch->Width = 210;
		tbKahouSearch->Height = 24;
		tbKahouSearch->Left = 10;
		tbKahouSearch->Top = 10;

		// 検索ボタン
		btnKahouSearch = gcnew Button();
		btnKahouSearch->Text = "検索";
		btnKahouSearch->Width = 90;
		btnKahouSearch->Height = tbKahouSearch->Height;
		btnKahouSearch->Left = tbKahouSearch->Right + 1;
		btnKahouSearch->Top = tbKahouSearch->Top;

		// 取込ボタン
		btnKahouImport = gcnew Button();
		btnKahouImport->Text = "単独取込";
		btnKahouImport->Width = 105;
		btnKahouImport->Height = tbKahouSearch->Height;
		btnKahouImport->Left = btnKahouSearch->Right + 45;
		btnKahouImport->Top = tbKahouSearch->Top;

		// 出力ボタン
		btnKahouExport = gcnew Button();
		btnKahouExport->Text = "単独出力";
		btnKahouExport->Width = 105;
		btnKahouExport->Height = tbKahouSearch->Height;
		btnKahouExport->Left = btnKahouImport->Right;
		btnKahouExport->Top = tbKahouSearch->Top;

		// 全て保存
		btnKahouAllSave = gcnew Button();
		btnKahouAllSave->Text = "全て保存";
		btnKahouAllSave->Width = 105;
		btnKahouAllSave->Height = tbKahouSearch->Height;
		btnKahouAllSave->Left = tpKahou->Right - (btnKahouAllSave->Width+30);
		btnKahouAllSave->Top = tbKahouSearch->Top;

		// 家宝知ると
		lbKahouList = gcnew ListBox();
		lbKahouList->Width = tbKahouSearch->Width;
		lbKahouList->Left = tbKahouSearch->Left;
		lbKahouList->Top = tbKahouSearch->Bottom + 15;
		lbKahouList->Height = 300;

		// 列伝情報用のパネル
		plKahouRetsuden = gcnew Panel();
		plKahouRetsuden->Top = lbKahouList->Top;
		Kahou_SetPanelBackImg();
		plKahouRetsuden->Left = lbKahouList->Right + 10;
		plKahouRetsuden->Width = 600;
		plKahouRetsuden->Height = 350;

		tbKahouFullName = gcnew TextBox();
		tbKahouFullName->Top = 15;
		tbKahouFullName->Left = 15;
		tbKahouFullName->Width = 160;
		tbKahouFullName->MaxLength = 37;
		tbKahouFullName->BackColor = Color::FromArgb(0xC0, 0xD0, 0xF0);

		tbKahouRubyYomi = gcnew TextBox();
		tbKahouRubyYomi->Top = tbKahouFullName->Top;
		tbKahouRubyYomi->Left = tbKahouFullName->Right + 10;
		tbKahouRubyYomi->MaxLength = 37;
		tbKahouRubyYomi->Width = 160;
		tbKahouRubyYomi->BackColor = Color::FromArgb(0xC0, 0xD0, 0xF0);

		tbKahouBornEtc  = gcnew TextBox();
		tbKahouBornEtc->Top = tbKahouFullName->Top;
		tbKahouBornEtc->Left = tbKahouRubyYomi->Right + 10;
		tbKahouBornEtc->Width = 160;
		tbKahouBornEtc->MaxLength = 37;
		tbKahouBornEtc->BackColor = Color::FromArgb(0xC0, 0xD0, 0xF0);

		tbKahouRetsuden = gcnew TextBox();
		tbKahouRetsuden->Top = tbKahouFullName->Bottom + 12;
		tbKahouRetsuden->Left = tbKahouFullName->Left;
		tbKahouRetsuden->Height = 135;
		tbKahouRetsuden->Width = (int)(312 * 1.5 + 32);
		tbKahouRetsuden->MaxLength = 2 * 27 * 4;
		tbKahouRetsuden->Multiline = true;
		tbKahouRetsuden->WordWrap = false;
		tbKahouRetsuden->BackColor = Color::FromArgb(0xC0, 0xD0, 0xF0);
		

		plKahouRetsuden->Controls->Add(tbKahouFullName);
		plKahouRetsuden->Controls->Add(tbKahouRubyYomi);
		// plKahouRetsuden->Controls->Add(tbKahouBornEtc);
		plKahouRetsuden->Controls->Add(tbKahouRetsuden);

		// フォントリスト
		cbFontKahou = gcnew ComboBox();
		cbFontKahou->Width = 150;
		cbFontKahou->Left = lbKahouList->Right + 10;
		cbFontKahou->Top = plKahouRetsuden->Bottom + 15;
		cbFontKahou->Height = tbKahouSearch->Height;
		Kahou_GetValidTenshoukiFontList();

		Kahou_cbFont_SelectedIndexChanged(nullptr, nullptr);

		// 家宝枠50人追加
		btnKahouAdds = gcnew Button();
		btnKahouAdds->Text = "50枠追加";
		btnKahouAdds->Width = 70;
		btnKahouAdds->Height = tbKahouSearch->Height;
		btnKahouAdds->Left = tbKahouSearch->Left;
		btnKahouAdds->Top = lbKahouList->Bottom + 10;

		// 家宝枠50人削除
		btnKahouDels = gcnew Button();
		btnKahouDels->Text = "50枠削除";
		btnKahouDels->Width = 70;
		btnKahouDels->Height = tbKahouSearch->Height;
		btnKahouDels->Left = btnKahouAdds->Right;
		btnKahouDels->Top = lbKahouList->Bottom + 10;


		// 家宝列伝のタブにコンポーネント追加
		tpKahou->Controls->Add(tbKahouSearch);
		tpKahou->Controls->Add(btnKahouSearch);
		// tpKahou->Controls->Add(btnKahouImport);
		// tpKahou->Controls->Add(btnKahouExport);
		tpKahou->Controls->Add(btnKahouAllSave);
		tpKahou->Controls->Add(lbKahouList);
		tpKahou->Controls->Add(plKahouRetsuden);
		// tpKahou->Controls->Add(cbFontKahou);
		// tpKahou->Controls->Add(btnKahouAdds); // 家宝用にはこのボタンは不要
		// tpKahou->Controls->Add(btnKahouDels); // 家宝用にはこのボタンは不要
		
		// タブをフォームに追加
		tcRE->TabPages->Add(tpKahou);

		// 家宝データを作成して、リストに流しこむ
		bdKahouData = gcnew KahouData();
		// リストボックスに足し込み
		for ( int i=0; i<bdKahouData->lstStrFullName->Count; i++ ) {
			String^ strLine = String::Format( "{0:000} - {1}", i, bdKahouData->lstStrFullName[i]);
			lbKahouList->Items->Add( strLine );
		}

		// 最初のものを選択しておく
		lbKahouList->SelectedIndex = 0;
		iLastKahouListSelected = 0;

		// 家宝データ→テキストボックス
		Kahou_BDataToTextBox();
		Kahou_TextBoxWidthUpdate();
	}

	// パネルの背景画像設定
	void Kahou_SetPanelBackImg() {
		return; 
		// このプロジェクトのアセンブリのタイプを取得。
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		// パネルの背景
		plKahouRetsuden->BackgroundImage = (System::Drawing::Image^)(r->GetObject("retsuden_back"));
		// 背景画像の大きさにパネルの大きさを合わせる
		plKahouRetsuden->Size = plKahouRetsuden->BackgroundImage->Size;

	}

	// 各種ＧＵＩのイベントハンドラ設定
	void Kahou_SetEventHander() {
		btnKahouSearch->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_btnSearch_Click);
		btnKahouImport->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_btnImport_Click);
		btnKahouExport->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_btnExport_Click);
		btnKahouAllSave->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_btnAllSave_Click);
		tbKahouSearch->KeyDown += gcnew KeyEventHandler(this, &RetsudenEditorForm::Kahou_tbSearch_KeyDown);
		lbKahouList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_lbKahouList_SelectedIndexChanged);
		tbKahouFullName->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Kahou_tbFullName_TextChanged);
		tbKahouRubyYomi->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Kahou_tbRubyYomi_TextChanged);
		tbKahouBornEtc->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Kahou_tbBornEtc_TextChanged);
		tbKahouRetsuden->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Kahou_tbRetsuden_TextChanged);
		tbKahouFullName->Leave += gcnew EventHandler( this, &RetsudenEditorForm::Kahou_textBox_Leave);
		tbKahouRubyYomi->Leave += gcnew EventHandler( this, &RetsudenEditorForm::Kahou_textBox_Leave);
		tbKahouBornEtc->Leave += gcnew EventHandler( this, &RetsudenEditorForm::Kahou_textBox_Leave);
		tbKahouRetsuden->Leave += gcnew EventHandler( this, &RetsudenEditorForm::Kahou_textBox_Leave);
		cbFontKahou->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_cbFont_SelectedIndexChanged);
		btnKahouAdds->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_btnAddKahous_Click);
		btnKahouDels->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_btnDelKahous_Click);
	}

	// 家宝検索ボタンを押した時
	void Kahou_btnSearch_Click(Object^ sender, EventArgs^ e) {
		if ( tbKahouSearch->Text == "" ) { return; } // テキストエリアが空っぽなら何もしない。

		int iStart = GetKahouCBSelectedIndex() + 1; // 現在選択しているもののから次の候補

		Regex^ regex = gcnew Regex(tbKahouSearch->Text);

		bool isExist = false;
		// iStart以降を全部サーチして、
		for ( int i=iStart; i<lbKahouList->Items->Count; i++) {
			Match^ match = regex->Match( (String^)lbKahouList->Items[i] );

			if ( match->Success ) {
				lbKahouList->SelectedIndex = i;
				isExist = true;
				break;
			}
		}

		if ( !isExist ) {
			for ( int i=0; i<iStart; i++) {
				Match^ match = regex->Match( (String^)lbKahouList->Items[i] );

				if ( match->Success ) {
					lbKahouList->SelectedIndex = i;
					break;
				}
			}
		}
	}

	// 家宝検索テキストボックスでキーを押した時
	void Kahou_tbSearch_KeyDown(Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Return ) {
			Kahou_btnSearch_Click(sender, e);
		}
		if (e->KeyCode == Keys::F3 ) {
			Kahou_btnSearch_Click(sender, e);
		}
	}

	// 家宝出力ボタンを押した時
	void Kahou_btnExport_Click(Object^ sender, EventArgs^ e) {
	}


	// 家宝入力ボタンを押した時
	void Kahou_btnImport_Click(Object^ sender, EventArgs^ e) {
	}

	// 全て保存ボタンを押した時
	void Kahou_btnAllSave_Click(Object^ sender, EventArgs^ e) {
		bdKahouData->AllSaveToMessageN6P();
	}

	// フォントリストの選択項目が変更された時
	void Kahou_cbFont_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		// フォントの設定
		fontKahou = gcnew System::Drawing::Font((String^)cbFontKahou->Items[cbFontKahou->SelectedIndex], 12, FontStyle::Regular);
		tbKahouFullName->Font = fontKahou;
		tbKahouRubyYomi->Font = fontKahou;
		tbKahouBornEtc->Font  = fontKahou;
		tbKahouRetsuden->Font = fontKahou;
	}

	// インストールされているフォントにしたがって、フォントリストボックスに追加
	void Kahou_GetValidTenshoukiFontList() {

		//InstalledFontCollectionオブジェクトの取得
		System::Drawing::Text::InstalledFontCollection^ ifc = gcnew System::Drawing::Text::InstalledFontCollection();

		//インストールされているすべてのフォントファミリアを取得
		cli::array<FontFamily^>^ ffs = ifc->Families;

		//ここでは候補となりえるフォント名のみ
		for ( int i=0; i<aryStrFontCandidate->Length; i++) {

			// フォントリスト
			for each (FontFamily^ ff in ffs) {

				// 候補フォントがあった。
				if (ff->Name == aryStrFontCandidate[i]) {
					// コンボボックスに追加
					cbFontKahou->Items->Add( ff->Name );
				}
			}
		}

		// １つ以上フォントがあったら、最初のものを選択しておく。
		if ( cbFontKahou->Items->Count > 0 ) {
			cbFontKahou->SelectedIndex = 0;
		}
	}

	int GetKahouCBSelectedIndex() {
		int iSelectedIndex = lbKahouList->SelectedIndex;
		if ( 0 <= iSelectedIndex && iSelectedIndex < lbKahouList->Items->Count) {
			return iSelectedIndex;
		} else {
			return 0;
		}
	}
	void ModifyCBKahouSelectedIndex() {
		// 焦点を見失っているようなら、最後に選択していたものを宛がう。
		int iSelectIndex = GetKahouCBSelectedIndex();
		if ( iSelectIndex == 0xFFFFFFFF ) {
			lbKahouList->SelectedIndex = iLastKahouListSelected;
		} else {
			iLastKahouListSelected = iSelectIndex;
		}
	}


	// 家宝リストを選択変更すると、画像の上の列伝各種の値が入れ替わる
	void Kahou_lbKahouList_SelectedIndexChanged(Object^ sender, EventArgs^ e ) {
		ModifyCBKahouSelectedIndex();
		// 新たなindexのデータを取得。
		Kahou_BDataToTextBox();

		ModifyCBKahouSelectedIndex();
	}


	// 家宝データ→テキストボックスへ転写
	void Kahou_BDataToTextBox() {
		int iSelectedIndex = GetKahouCBSelectedIndex();
		tbKahouFullName->Text = bdKahouData->lstStrFullName[iSelectedIndex];
		tbKahouRubyYomi->Text = bdKahouData->lstStrRubyYomi[iSelectedIndex];
		tbKahouBornEtc->Text  = bdKahouData->lstStrBornEtc[iSelectedIndex];
		tbKahouRetsuden->Text = bdKahouData->lstStrRetsuden[iSelectedIndex];
	}

	// テキストボックスの位置や幅を中身のテキスト量に従って変更
	void Kahou_TextBoxWidthUpdate() {
		return;

		tbKahouFullName->Width = GetStringMeasure(tbKahouFullName->Text);

		tbKahouRubyYomi->Left = tbKahouFullName->Right + 8;
		tbKahouRubyYomi->Width = GetStringMeasure(tbKahouRubyYomi->Text);
		
		tbKahouBornEtc->Left = tbKahouRubyYomi->Right + 8;
		tbKahouBornEtc->Width = GetStringMeasure(tbKahouBornEtc->Text);
	}

	// テキストボックスの、「家宝姓名」が変更された。
	void Kahou_tbFullName_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetKahouCBSelectedIndex();
		bdKahouData->lstStrFullName[iSelectedIndex] = tbKahouFullName->Text;
		String^ strLine = String::Format( "{0:000} - {1}", iSelectedIndex, tbKahouFullName->Text);
		// SelectIndexChangeが反応しないようにする。
		lbKahouList->SelectedIndexChanged -= gcnew EventHandler(this, &RetsudenEditorForm::Kahou_lbKahouList_SelectedIndexChanged);

		// 反応しないようにしたので、値を書き換える。SelectedIndexChangeメソッドが反応しないのでエラーが起きない。

		lbKahouList->Items[iSelectedIndex] = gcnew String(strLine); // 家宝リストの名前の方を更新

		Kahou_TextBoxWidthUpdate();

		// 更新したので、反応を戻す。
		// SelectIndexChangeが反応するようにする。
		lbKahouList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_lbKahouList_SelectedIndexChanged);
	}

	// テキストボックスの、「ふりがな」が変更された。
	void Kahou_tbRubyYomi_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetKahouCBSelectedIndex();
		bdKahouData->lstStrRubyYomi[iSelectedIndex] = tbKahouRubyYomi->Text;
		Kahou_TextBoxWidthUpdate();
	}

	// テキストボックスの、「生年等」が変更された。
	void Kahou_tbBornEtc_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetKahouCBSelectedIndex();
		bdKahouData->lstStrBornEtc[iSelectedIndex]  = tbKahouBornEtc->Text;
		Kahou_TextBoxWidthUpdate();
	}

	// テキストボックスの、「列伝」が変更された。
	void Kahou_tbRetsuden_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetKahouCBSelectedIndex();

		bdKahouData->lstStrRetsuden[iSelectedIndex] = tbKahouRetsuden->Text;
	}

	// テキストボックスから去った
	void Kahou_textBox_Leave(Object^ sender, EventArgs^ e ) {
	}


	// 50枠追加ボタン
	void Kahou_btnAddKahous_Click(Object^ sender, EventArgs^ e) {
	}

	// 50枠削除ボタン
	void Kahou_btnDelKahous_Click(Object^ sender, EventArgs^ e) {
	}

//-----------------------------------------------城列伝系
private:
	TabPage^ tpCastle;
	CastleData^ bdCastleData;

	TextBox^ tbCastleSearch; // 城検索
	Button^ btnCastleSearch; // 検索ボタン
	Button^ btnCastleImport; // 取込ボタン
	Button^ btnCastleExport; // 出力ボタン
	Button^ btnCastleAllSave; // 全て保存

	TreeView^ tvCastleList; // 城リストのツリービュー
	int iLastCastleListSelected;
	List<TreeNode^>^ refTvCastleListNodes; // 城リストのツリービュー内にあるノードのリファレンス

	Panel^ plCastleRetsuden;	 // 列伝情報用のパネル
	TextBox^ tbCastleFullName; // 城姓名のテキストボックス
	TextBox^ tbCastleRubyYomi; // ふりがなのるび
	TextBox^ tbCastleBornEtc;  // 生年などの情報
	TextBox^ tbCastleRetsuden; // 列伝情報


	// フォント
	System::Drawing::Font^ fontCastle;
	ComboBox^ cbFontCastle;	// フォントリスト

	Button^ btnCastleAdds; // 50枠追加
	Button^ btnCastleDels; // 50枠追加


private:

	void tpCastle_Init() {

		tpCastle = gcnew TabPage();
		tpCastle->BackColor = Color::Transparent;

		tpCastle->Text = "城列伝";
		tpCastle->Size = tcRE->ClientSize;

		// 城検索
		tbCastleSearch = gcnew TextBox();
		tbCastleSearch->Width = 210;
		tbCastleSearch->Height = 24;
		tbCastleSearch->Left = 10;
		tbCastleSearch->Top = 10;

		// 検索ボタン
		btnCastleSearch = gcnew Button();
		btnCastleSearch->Text = "検索";
		btnCastleSearch->Width = 90;
		btnCastleSearch->Height = tbCastleSearch->Height;
		btnCastleSearch->Left = tbCastleSearch->Right + 1;
		btnCastleSearch->Top = tbCastleSearch->Top;

		// 取込ボタン
		btnCastleImport = gcnew Button();
		btnCastleImport->Text = "単独取込";
		btnCastleImport->Width = 105;
		btnCastleImport->Height = tbCastleSearch->Height;
		btnCastleImport->Left = btnCastleSearch->Right + 45;
		btnCastleImport->Top = tbCastleSearch->Top;

		// 出力ボタン
		btnCastleExport = gcnew Button();
		btnCastleExport->Text = "単独出力";
		btnCastleExport->Width = 105;
		btnCastleExport->Height = tbCastleSearch->Height;
		btnCastleExport->Left = btnCastleImport->Right;
		btnCastleExport->Top = tbCastleSearch->Top;

		// 全て保存
		btnCastleAllSave = gcnew Button();
		btnCastleAllSave->Text = "全て保存";
		btnCastleAllSave->Width = 105;
		btnCastleAllSave->Height = tbCastleSearch->Height;
		btnCastleAllSave->Left = tpCastle->Right - (btnCastleAllSave->Width+30);
		btnCastleAllSave->Top = tbCastleSearch->Top;

		// 城知ると
		tvCastleList = gcnew TreeView();
		tvCastleList->HideSelection = false;
		tvCastleList->Width = tbCastleSearch->Width;
		tvCastleList->Left = tbCastleSearch->Left;
		tvCastleList->Top = tbCastleSearch->Bottom + 15;
		tvCastleList->Height = 300;

		// 列伝情報用のパネル
		plCastleRetsuden = gcnew Panel();
		plCastleRetsuden->Top = tvCastleList->Top;
		Castle_SetPanelBackImg();
		plCastleRetsuden->Left = tvCastleList->Right + 10;
		plCastleRetsuden->Width = 600;
		plCastleRetsuden->Height = 350;

		tbCastleFullName = gcnew TextBox();
		tbCastleFullName->Top = 15;
		tbCastleFullName->Left = 15;
		tbCastleFullName->Width = 160;
		tbCastleFullName->MaxLength = 37;
		tbCastleFullName->BackColor = Color::FromArgb(0xC0, 0xD0, 0xF0);

		tbCastleRubyYomi = gcnew TextBox();
		tbCastleRubyYomi->Top = tbCastleFullName->Top;
		tbCastleRubyYomi->Left = tbCastleFullName->Right + 10;
		tbCastleRubyYomi->MaxLength = 37;
		tbCastleRubyYomi->Width = 160;
		tbCastleRubyYomi->BackColor = Color::FromArgb(0xC0, 0xD0, 0xF0);

		tbCastleBornEtc  = gcnew TextBox();
		tbCastleBornEtc->Top = tbCastleFullName->Top;
		tbCastleBornEtc->Left = tbCastleRubyYomi->Right + 10;
		tbCastleBornEtc->Width = 160;
		tbCastleBornEtc->MaxLength = 37;
		tbCastleBornEtc->BackColor = Color::FromArgb(0xC0, 0xD0, 0xF0);

		tbCastleRetsuden = gcnew TextBox();
		tbCastleRetsuden->Top = tbCastleFullName->Bottom + 12;
		tbCastleRetsuden->Left = tbCastleFullName->Left;
		tbCastleRetsuden->Height = 135;
		tbCastleRetsuden->Width = (int)(312 * 1.5 + 32);
		tbCastleRetsuden->MaxLength = 2 * 27 * 4;
		tbCastleRetsuden->Multiline = true;
		tbCastleRetsuden->WordWrap = false;
		tbCastleRetsuden->BackColor = Color::FromArgb(0xC0, 0xD0, 0xF0);
		

		plCastleRetsuden->Controls->Add(tbCastleFullName);
		plCastleRetsuden->Controls->Add(tbCastleRubyYomi);
		// plCastleRetsuden->Controls->Add(tbCastleBornEtc);
		plCastleRetsuden->Controls->Add(tbCastleRetsuden);

		// フォントリスト
		cbFontCastle = gcnew ComboBox();
		cbFontCastle->Width = 100;
		cbFontCastle->Left = tvCastleList->Right + 10;
		cbFontCastle->Top = plCastleRetsuden->Bottom + 15;
		cbFontCastle->Height = tbCastleSearch->Height;
		Castle_GetValidTenshoukiFontList();

		Castle_cbFont_SelectedIndexChanged(nullptr, nullptr);

		// 城枠50人追加
		btnCastleAdds = gcnew Button();
		btnCastleAdds->Text = "50枠追加";
		btnCastleAdds->Width = 70;
		btnCastleAdds->Height = tbCastleSearch->Height;
		btnCastleAdds->Left = tbCastleSearch->Left;
		btnCastleAdds->Top = tvCastleList->Bottom + 10;

		// 城枠50人削除
		btnCastleDels = gcnew Button();
		btnCastleDels->Text = "50枠削除";
		btnCastleDels->Width = 70;
		btnCastleDels->Height = tbCastleSearch->Height;
		btnCastleDels->Left = btnCastleAdds->Right;
		btnCastleDels->Top = tvCastleList->Bottom + 10;


		// 城列伝のタブにコンポーネント追加
		tpCastle->Controls->Add(tbCastleSearch);
		tpCastle->Controls->Add(btnCastleSearch);
		// tpCastle->Controls->Add(btnCastleImport);
		// tpCastle->Controls->Add(btnCastleExport);
		tpCastle->Controls->Add(btnCastleAllSave);
		tpCastle->Controls->Add(tvCastleList);
		tpCastle->Controls->Add(plCastleRetsuden);
		// tpCastle->Controls->Add(cbFontCastle);
		// tpCastle->Controls->Add(btnCastleAdds); // 城用にはこのボタンは不要
		// tpCastle->Controls->Add(btnCastleDels); // 城用にはこのボタンは不要
		
		// タブをフォームに追加
		tcRE->TabPages->Add(tpCastle);

		// 城データを作成して、リストに流しこむ
		bdCastleData = gcnew CastleData();

		// リファレンス用のため。走査しやすくするため。
		refTvCastleListNodes = gcnew List<TreeNode^>();

		// ツリービューに要素を足し込み
		Castle_AddElementTreeView();

		// 最初のものを選択しておく
		iLastCastleListSelected = 0;

		// 城データ→テキストボックス
		Castle_BDataToTextBox();
		Castle_TextBoxWidthUpdate();
	}

	// ★ツリービューに要素を足し込み
	void Castle_AddElementTreeView() {
		// リストボックスに足し込み
		for ( int i=0; i<bdCastleData->lstStrFullName->Count; i++ ) {
			String^ strLine = String::Format( "{0:000} - {1}", i, bdCastleData->lstStrFullName[i]);

			TreeNode^ treeNode;
			List<TreeNode^>^ treeNodeNestList = gcnew List<TreeNode^>();

			// ２つ以上の城が１セットになっている場合。サブにノードをくっつける
			if ( vCastleInfoListData[i].size() >= 2) { 
				cli::array<TreeNode^>^ treeSubNodeList = gcnew cli::array<TreeNode^>( vCastleInfoListData[i].size()-1 ); 
				for (int j=1; j<(int)vCastleInfoListData[i].size(); j++) {
					TreeNode^ treeNode = gcnew TreeNode( gcnew String(vCastleInfoListData[i][j].szFullName) );
					treeSubNodeList[j-1] = treeNode;

					// 識別のため、タグを入れておく
					List<int>^ pair = gcnew List<int>;
					pair->Add(i);
					pair->Add(j);
					treeNode->Tag = pair;

					treeNodeNestList->Add(treeNode);

				}
				treeNode = gcnew TreeNode(strLine, treeSubNodeList); // 複数のノードを子供としてくっつける。
				treeNode->ExpandAll();

			} else {
				treeNode = gcnew TreeNode(strLine);
			}

			// 識別のため、タグを入れておく
			List<int>^ pair = gcnew List<int>;
			pair->Add(i);
			pair->Add(0);
			treeNode->Tag = pair;

			// 平べったくした状態で検索しやすいように、リファレンス用にも足しておく
			refTvCastleListNodes->Add(treeNode);
			// ネストした分をここで足す。
			for ( int j=0; j<treeNodeNestList->Count; j++ ) {
				// 平べったくした状態で検索しやすいように、リファレンス用にも足しておく
				refTvCastleListNodes->Add(treeNodeNestList[j]);
			}
			// TreeViewerに足しておく
			tvCastleList->Nodes->Add( treeNode );

			// 初めてたしたら、それフォーカス
			if (i==0) {
				tvCastleList->SelectedNode = treeNode;
			}
		}

	}

	// ★パネルの背景画像設定
	void Castle_SetPanelBackImg() {
		return;
		// このプロジェクトのアセンブリのタイプを取得。
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		// パネルの背景
		plCastleRetsuden->BackgroundImage = (System::Drawing::Image^)(r->GetObject("retsuden_back"));
		// 背景画像の大きさにパネルの大きさを合わせる
		plCastleRetsuden->Size = plCastleRetsuden->BackgroundImage->Size;

	}

	// ★各種ＧＵＩのイベントハンドラ設定
	void Castle_SetEventHander() {
		btnCastleSearch->Click += gcnew EventHandler(this, &RetsudenEditorForm::Castle_btnSearch_Click);
		btnCastleAllSave->Click += gcnew EventHandler(this, &RetsudenEditorForm::Castle_btnAllSave_Click);
		tbCastleSearch->KeyDown += gcnew KeyEventHandler(this, &RetsudenEditorForm::Castle_tbSearch_KeyDown);
		tbCastleFullName->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Castle_tbFullName_TextChanged);
		tbCastleRubyYomi->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Castle_tbRubyYomi_TextChanged);
		tbCastleRetsuden->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Castle_tbRetsuden_TextChanged);
		cbFontCastle->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Castle_cbFont_SelectedIndexChanged);
		tvCastleList->AfterSelect += gcnew TreeViewEventHandler(this, &RetsudenEditorForm::Castle_tvCastleList_AfterSelect);
	}

	// ★城検索ボタンを押した時
	void Castle_btnSearch_Click(Object^ sender, EventArgs^ e) {

		if ( tbCastleSearch->Text == "" ) { return; } // テキストエリアが空っぽなら何もしない。

		int iStart = GetCastleFlatSelectedIndex() + 1; // 現在選択しているもののから次の候補

		Regex^ regex = gcnew Regex(tbCastleSearch->Text);

		bool isExist = false;
		// iStart以降を全部サーチして、
		for ( int i=iStart; i<refTvCastleListNodes->Count; i++) {
			Match^ match = regex->Match( refTvCastleListNodes[i]->Text );

			if ( match->Success ) {
				tvCastleList->SelectedNode = refTvCastleListNodes[i];
				isExist = true;
				break;
			}
		}

		if ( !isExist ) {
			for ( int i=0; i<iStart; i++) {
				Match^ match = regex->Match( refTvCastleListNodes[i]->Text );

				if ( match->Success ) {
					tvCastleList->SelectedNode = refTvCastleListNodes[i];
					break;
				}
			}
		}
	}

	// ★城検索テキストボックスでキーを押した時
	void Castle_tbSearch_KeyDown(Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Return ) {
			Castle_btnSearch_Click(sender, e);
		}
		if (e->KeyCode == Keys::F3 ) {
			Castle_btnSearch_Click(sender, e);
		}
	}

	// ★全て保存ボタンを押した時
	void Castle_btnAllSave_Click(Object^ sender, EventArgs^ e) {
		bdCastleData->AllSaveToMessageN6P();
	}

	// ★フォントリストの選択項目が変更された時
	void Castle_cbFont_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		// フォントの設定
		fontCastle = gcnew System::Drawing::Font((String^)cbFontCastle->Items[cbFontCastle->SelectedIndex], 12, FontStyle::Regular);
		tbCastleFullName->Font = fontCastle;
		tbCastleRubyYomi->Font = fontCastle;
		tbCastleBornEtc->Font  = fontCastle;
		tbCastleRetsuden->Font = fontCastle;
	}

	// ★インストールされているフォントにしたがって、フォントリストボックスに追加
	void Castle_GetValidTenshoukiFontList() {

		//InstalledFontCollectionオブジェクトの取得
		System::Drawing::Text::InstalledFontCollection^ ifc = gcnew System::Drawing::Text::InstalledFontCollection();

		//インストールされているすべてのフォントファミリアを取得
		cli::array<FontFamily^>^ ffs = ifc->Families;

		//ここでは候補となりえるフォント名のみ
		for ( int i=0; i<aryStrFontCandidate->Length; i++) {

			// フォントリスト
			for each (FontFamily^ ff in ffs) {

				// 候補フォントがあった。
				if (ff->Name == aryStrFontCandidate[i]) {
					// コンボボックスに追加
					cbFontCastle->Items->Add( ff->Name );
				}
			}
		}

		// １つ以上フォントがあったら、最初のものを選択しておく。
		if ( cbFontCastle->Items->Count > 0 ) {
			cbFontCastle->SelectedIndex = 0;
		}
	}

	// ★(平たくした方で)、選択しているノードのIDを得る。選択してなければ、0番
	int GetCastleFlatSelectedIndex() {
		
		TreeNode^ iSelectedNode = tvCastleList->SelectedNode;
		for ( int i=0; i < refTvCastleListNodes->Count; i++ ) {
			if ( iSelectedNode == refTvCastleListNodes[i] ) {
				return i;
			}
		}
		return 0;
	}


	// ★ツリーのどれかが選択された時
	void Castle_tvCastleList_AfterSelect(Object^ sender, TreeViewEventArgs^ e) {
		// 新たなindexのデータを取得。
		Castle_BDataToTextBox();
	}


	// ★城データ→テキストボックスへ転写
	void Castle_BDataToTextBox() {
		TreeNode^ tnSelectedNode = tvCastleList->SelectedNode;
		List<int>^ pair = (List<int>^)tnSelectedNode->Tag;
		int iMainIndex = pair[0];
		int iSubIndex = pair[1];
		tbCastleFullName->Text = gcnew String(vCastleInfoListData[iMainIndex][iSubIndex].szFullName);
		tbCastleRubyYomi->Text = gcnew String(vCastleInfoListData[iMainIndex][iSubIndex].szRubyYomi);
		tbCastleRetsuden->Text = gcnew String(vCastleRetsudenInfoListData[iMainIndex][iSubIndex].szRetsuden);
	}

	// ★テキストボックスの位置や幅を中身のテキスト量に従って変更
	void Castle_TextBoxWidthUpdate() {
		return;

		tbCastleFullName->Width = GetStringMeasure(tbCastleFullName->Text);

		tbCastleRubyYomi->Left = tbCastleFullName->Right + 8;
		tbCastleRubyYomi->Width = GetStringMeasure(tbCastleRubyYomi->Text);
		
		tbCastleBornEtc->Left = tbCastleRubyYomi->Right + 8;
		tbCastleBornEtc->Width = GetStringMeasure(tbCastleBornEtc->Text);
	}

	// ★テキストボックスの、「城姓名」が変更された。
	void Castle_tbFullName_TextChanged(Object^ sender, EventArgs^ e ) {
		// 選択ノードのID
		TreeNode^ tnSelectedNode = tvCastleList->SelectedNode;
		List<int>^ pair = (List<int>^)tnSelectedNode->Tag;
		int iMainIndex = pair[0];
		int iSubIndex = pair[1];

		string strFullName = DotNetStringToMultiByte( tbCastleFullName->Text );
		strcpy( vCastleInfoListData[iMainIndex][iSubIndex].szFullName, strFullName.c_str() );

		String^ strLine;
		if ( iSubIndex == 0 ) {
			strLine = String::Format( "{0:000} - {1}", iMainIndex, tbCastleFullName->Text);
		} else {
			strLine = String::Format( "{0}", tbCastleFullName->Text);
		}
		tnSelectedNode->Text = strLine;

		Castle_TextBoxWidthUpdate();
	}

	// ★テキストボックスの、「ふりがな」が変更された。
	void Castle_tbRubyYomi_TextChanged(Object^ sender, EventArgs^ e ) {
		// 選択ノードのID
		TreeNode^ tnSelectedNode = tvCastleList->SelectedNode;
		List<int>^ pair = (List<int>^)tnSelectedNode->Tag;
		int iMainIndex = pair[0];
		int iSubIndex = pair[1];

		string strRubyYomi = DotNetStringToMultiByte( tbCastleRubyYomi->Text );
		strcpy( vCastleInfoListData[iMainIndex][iSubIndex].szRubyYomi, strRubyYomi.c_str() );
		Castle_TextBoxWidthUpdate();
	}


	// ★テキストボックスの、「列伝」が変更された。
	void Castle_tbRetsuden_TextChanged(Object^ sender, EventArgs^ e ) {
		// 選択ノードのID
		TreeNode^ tnSelectedNode = tvCastleList->SelectedNode;
		List<int>^ pair = (List<int>^)tnSelectedNode->Tag;
		int iMainIndex = pair[0];
		int iSubIndex = pair[1];

		string strRetsuden = DotNetStringToMultiByte( tbCastleRetsuden->Text );
		strcpy( vCastleRetsudenInfoListData[iMainIndex][iSubIndex].szRetsuden, strRetsuden.c_str() );
		Castle_TextBoxWidthUpdate();
	}

};

