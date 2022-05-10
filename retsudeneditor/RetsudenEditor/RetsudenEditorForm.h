#pragma once;

#include "BushouData.h"
#include "KahouData.h"
#include "CastleData.h"

// RetsudenEditor.cpp : ���C�� �v���W�F�N�g �t�@�C���ł��B
#include <windows.h>
#include <string>

#include "DotNetTestToMultiByte.h"

ref class RetsudenEditorForm : public Form {

//-----------------------------------------------�t�H�[���E�B���h�E�n

	TabControl^ tcRE;	// �S�̂łP�ɂȂ��Ă�^�u�R���g���[���BTabPage�^��ǉ����Ă�

    static cli::array<String^>^ aryStrFontCandidate = {"�V�� ����", "�l�r ����", "�l�r �S�V�b�N" }; // �t�H���g���X�g

public:
	RetsudenEditorForm() {
		this->AutoScaleMode = ::AutoScaleMode::Dpi;

		// �t�H�[���ɂ��������̂̏�������
		this->Text = "HD�� ��`�G�f�B�^";
		this->Width = 800;
		this->Height = 500;

		SetFormIcon();

		SetTabControl();
	}

	void SetFormIcon() {
		// ���̃v���W�F�N�g�̃A�Z���u���̃^�C�v���擾�B
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		System::Drawing::Icon^ iconform = (System::Drawing::Icon^)(r->GetObject("icon"));
		this->Icon = iconform;
	}

	// �^�u�R���g���[���̏������B�ԂɁA�e�f�[�^�O���b�h�r���[�ւ̃f�[�^�������ݏ��������荞��
	void SetTabControl() {
		tcRE = gcnew TabControl();
		tcRE->Size = this->ClientSize;
		
		// ������`�̃^�u�y�[�W�ǉ�
		tpBushou_Init();
		Bushou_SetEventHander();

		// �ƕ��`�̃^�u�y�[�W�ǉ�
		tpKahou_Init();
		Kahou_SetEventHander();

		// ���`�̃^�u�y�[�W�ǉ�
		tpCastle_Init();
		Castle_SetEventHander();

		tcRE->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::tcRE_SelectedIndexChanged);
		this->Controls->Add(tcRE);
	}

	// �^�u��؂�ւ�����
	void tcRE_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
	}


	int GetStringMeasure(String^ str) {
		char szBufMeasure[128];
		wsprintf( szBufMeasure, "%s", str );
		int iByteNum = strlen( szBufMeasure );
		return iByteNum * 8 + 8;
	}

//-----------------------------------------------������`�n
private:
	TabPage^ tpBushou;
	BushouData^ bdBushouData;

	TextBox^ tbBushouSearch; // ��������
	Button^ btnBushouSearch; // �����{�^��
	Button^ btnBushouImport; // �捞�{�^��
	OpenFileDialog^ ofdBushouImport;
	Button^ btnBushouExport; // �o�̓{�^��
	Button^ btnBushouAllSave; // �S�ĕۑ�

	ListBox^ lbBushouList; // �������X�g�̃R���{�{�b�N�X
	int iLastBushouListSelected;

	Panel^ plBushouRetsuden;	 // ��`���p�̃p�l��
	TextBox^ tbBushouFullName; // ���������̃e�L�X�g�{�b�N�X
	TextBox^ tbBushouRubyYomi; // �ӂ肪�Ȃ̂��
	TextBox^ tbBushouBornEtc;  // ���N�Ȃǂ̏��
	TextBox^ tbBushouRetsuden; // ��`���


	// �t�H���g
	System::Drawing::Font^ fontBushou;
	ComboBox^ cbFontBushou;	// �t�H���g���X�g

	Button^ btnBushouAdds; // 50�g�ǉ�
	Button^ btnBushouDels; // 50�g�ǉ�


private:

	void tpBushou_Init() {

		tpBushou = gcnew TabPage();

		tpBushou->Text = "������`";
		tpBushou->Size = tcRE->ClientSize;

		// ��������
		tbBushouSearch = gcnew TextBox();
		tbBushouSearch->Width = 210;
		tbBushouSearch->Height = 24;
		tbBushouSearch->Left = 10;
		tbBushouSearch->Top = 10;

		// �����{�^��
		btnBushouSearch = gcnew Button();
		btnBushouSearch->Text = "����";
		btnBushouSearch->Width = 90;
		btnBushouSearch->Height = tbBushouSearch->Height;
		btnBushouSearch->Left = tbBushouSearch->Right + 1;
		btnBushouSearch->Top = tbBushouSearch->Top;

		// �捞�{�^��
		btnBushouImport = gcnew Button();
		btnBushouImport->Text = "�P�Ǝ捞";
		btnBushouImport->Width = 105;
		btnBushouImport->Height = tbBushouSearch->Height;
		btnBushouImport->Left = btnBushouSearch->Right + 45;
		btnBushouImport->Top = tbBushouSearch->Top;

		// �o�̓{�^��
		btnBushouExport = gcnew Button();
		btnBushouExport->Text = "�P�Əo��";
		btnBushouExport->Width = 105;
		btnBushouExport->Height = tbBushouSearch->Height;
		btnBushouExport->Left = btnBushouImport->Right;
		btnBushouExport->Top = tbBushouSearch->Top;

		// �S�ĕۑ�
		btnBushouAllSave = gcnew Button();
		btnBushouAllSave->Text = "�S�ĕۑ�";
		btnBushouAllSave->Width = 105;
		btnBushouAllSave->Height = tbBushouSearch->Height;
		btnBushouAllSave->Left = tpBushou->Right - (btnBushouAllSave->Width+30);
		btnBushouAllSave->Top = tbBushouSearch->Top;

		// �����m���
		lbBushouList = gcnew ListBox();
		lbBushouList->Width = tbBushouSearch->Width;
		lbBushouList->Left = tbBushouSearch->Left;
		lbBushouList->Top = tbBushouSearch->Bottom + 15;
		lbBushouList->Height = 300;

		// ��`���p�̃p�l��
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

		// �t�H���g���X�g
		cbFontBushou = gcnew ComboBox();
		cbFontBushou->Width = 150;
		cbFontBushou->Left = lbBushouList->Right + 10;
		cbFontBushou->Top = plBushouRetsuden->Bottom + 15;
		cbFontBushou->Height = tbBushouSearch->Height;
		Bushou_GetValidTenshoukiFontList();

		Bushou_cbFont_SelectedIndexChanged(nullptr, nullptr);

		// �����g50�l�ǉ�
		btnBushouAdds = gcnew Button();
		btnBushouAdds->Text = "50�g�ǉ�";
		btnBushouAdds->Width = 70;
		btnBushouAdds->Height = tbBushouSearch->Height;
		btnBushouAdds->Left = tbBushouSearch->Left;
		btnBushouAdds->Top = lbBushouList->Bottom + 10;

		// �����g50�l�폜
		btnBushouDels = gcnew Button();
		btnBushouDels->Text = "50�g�폜";
		btnBushouDels->Width = 70;
		btnBushouDels->Height = tbBushouSearch->Height;
		btnBushouDels->Left = btnBushouAdds->Right;
		btnBushouDels->Top = lbBushouList->Bottom + 10;
		btnBushouAdds->Enabled = false;


		// ������`�̃^�u�ɃR���|�[�l���g�ǉ�
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
		
		// �^�u���t�H�[���ɒǉ�
		tcRE->TabPages->Add(tpBushou);

		// �����f�[�^���쐬���āA���X�g�ɗ�������
		bdBushouData = gcnew BushouData();
		// ���X�g�{�b�N�X�ɑ�������
		for ( int i=0; i<bdBushouData->lstStrFullName->Count; i++ ) {
			String^ strLine = String::Format( "{0:0000} - {1}", i, bdBushouData->lstStrFullName[i]);
			lbBushouList->Items->Add( strLine );
		}

		// �ŏ��̂��̂�I�����Ă���
		lbBushouList->SelectedIndex = 0;
		iLastBushouListSelected = 0;

		// �����f�[�^���e�L�X�g�{�b�N�X
		Bushou_BDataToTextBox();
		Bushou_TextBoxWidthUpdate();
	}

	// �p�l���̔w�i�摜�ݒ�
	void Bushou_SetPanelBackImg() {
		// �s�v
		return;

		// ���̃v���W�F�N�g�̃A�Z���u���̃^�C�v���擾�B
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		// �p�l���̔w�i
		plBushouRetsuden->BackgroundImage = (System::Drawing::Image^)(r->GetObject("retsuden_back"));
		// �w�i�摜�̑傫���Ƀp�l���̑傫�������킹��
		plBushouRetsuden->Size = plBushouRetsuden->BackgroundImage->Size;

	}

	// �e��f�t�h�̃C�x���g�n���h���ݒ�
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

	// ���������{�^������������
	void Bushou_btnSearch_Click(Object^ sender, EventArgs^ e) {
		if ( tbBushouSearch->Text == "" ) { return; } // �e�L�X�g�G���A������ۂȂ牽�����Ȃ��B

		int iStart = GetBushouCBSelectedIndex() + 1; // ���ݑI�����Ă�����̂̂��玟�̌��

		Regex^ regex = gcnew Regex(tbBushouSearch->Text);

		bool isExist = false;
		// iStart�ȍ~��S���T�[�`���āA
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

	// ���������e�L�X�g�{�b�N�X�ŃL�[����������
	void Bushou_tbSearch_KeyDown(Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Return ) {
			Bushou_btnSearch_Click(sender, e);
		}
		if (e->KeyCode == Keys::F3 ) {
			Bushou_btnSearch_Click(sender, e);
		}
	}

	// �����o�̓{�^������������
	void Bushou_btnExport_Click(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		std::string unmanagedData = bdBushouData->GetOneBushouExportData(iSelectedIndex);

		std::string filepath = DotNetStringToMultiByte(bdBushouData->lstStrFullName[iSelectedIndex]);
		if ( filepath == "" ) { filepath = "�ȂȂ�"; }
		filepath += ".n6b";

		FILE *fp = fopen(filepath.c_str(), "wb");
		if ( fp ) {
			fwrite( unmanagedData.c_str(), unmanagedData.size(), 1, fp);
			fclose(fp);
			System::Windows::Forms::MessageBox::Show("�f�[�^���o�͂��܂����B", "����", ::MessageBoxButtons::OK, ::MessageBoxIcon::Information);
		} else {
			System::Windows::Forms::MessageBox::Show("�f�[�^�̏o�͂Ɏ��s���܂����B", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);

		}
	}


	// �������̓{�^������������
	void Bushou_btnImport_Click(Object^ sender, EventArgs^ e) {

		ofdBushouImport = gcnew OpenFileDialog();

		ofdBushouImport->InitialDirectory = "."; // �c�[���Ɠ����f�B���N�g�����f�t�H���g

		ofdBushouImport->Filter = "�V�ċL������`�t�@�C��(*.n6b)|*.n6b";

		ofdBushouImport->FilterIndex = 1;

		ofdBushouImport->Title = "�Ƃ��镐���̗�`�t�@�C����I�����Ă�������";

		//�_�C�A���O�{�b�N�X�����O�Ɍ��݂̃f�B���N�g���𕜌�����悤�ɂ���
		ofdBushouImport->RestoreDirectory = true;

		//�_�C�A���O��\������
		if (ofdBushouImport->ShowDialog() == ::DialogResult::OK) {
			//OK�{�^�����N���b�N���ꂽ�Ƃ�
			if ( ofdBushouImport->FileName ) {
				BOOL success = bdBushouData->GetOneBushouImportData(ofdBushouImport->FileName, GetBushouCBSelectedIndex());

				//�@���ۂɂ�����炸�X�V
				Bushou_BDataToTextBox();
			}
		}
	}

	// �S�ĕۑ��{�^������������
	void Bushou_btnAllSave_Click(Object^ sender, EventArgs^ e) {
		bdBushouData->AllSaveToBFileN6P();
	}

	// �t�H���g���X�g�̑I�����ڂ��ύX���ꂽ��
	void Bushou_cbFont_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		// �t�H���g�̐ݒ�
		fontBushou = gcnew System::Drawing::Font((String^)cbFontBushou->Items[cbFontBushou->SelectedIndex], 12, FontStyle::Regular);
		tbBushouFullName->Font = fontBushou;
		tbBushouRubyYomi->Font = fontBushou;
		tbBushouBornEtc->Font  = fontBushou;
		tbBushouRetsuden->Font = fontBushou;
	}

	// �C���X�g�[������Ă���t�H���g�ɂ��������āA�t�H���g���X�g�{�b�N�X�ɒǉ�
	void Bushou_GetValidTenshoukiFontList() {

		//InstalledFontCollection�I�u�W�F�N�g�̎擾
		System::Drawing::Text::InstalledFontCollection^ ifc = gcnew System::Drawing::Text::InstalledFontCollection();

		//�C���X�g�[������Ă��邷�ׂẴt�H���g�t�@�~���A���擾
		cli::array<FontFamily^>^ ffs = ifc->Families;

		//�����ł͌��ƂȂ肦��t�H���g���̂�
		for ( int i=0; i<aryStrFontCandidate->Length; i++) {

			// �t�H���g���X�g
			for each (FontFamily^ ff in ffs) {

				// ���t�H���g���������B
				if (ff->Name == aryStrFontCandidate[i]) {
					// �R���{�{�b�N�X�ɒǉ�
					cbFontBushou->Items->Add( ff->Name );
				}
			}
		}

		// �P�ȏ�t�H���g����������A�ŏ��̂��̂�I�����Ă����B
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
		// �œ_���������Ă���悤�Ȃ�A�Ō�ɑI�����Ă������̂��������B
		int iSelectIndex = GetBushouCBSelectedIndex();
		if ( iSelectIndex == 0xFFFFFFFF ) {
			lbBushouList->SelectedIndex = iLastBushouListSelected;
		} else {
			iLastBushouListSelected = iSelectIndex;
		}
	}


	// �������X�g��I��ύX����ƁA�摜�̏�̗�`�e��̒l������ւ��
	void Bushou_lbBushouList_SelectedIndexChanged(Object^ sender, EventArgs^ e ) {
		ModifyCBBushouSelectedIndex();
		// �V����index�̃f�[�^���擾�B
		Bushou_BDataToTextBox();

		ModifyCBBushouSelectedIndex();
	}


	// �����f�[�^���e�L�X�g�{�b�N�X�֓]��
	void Bushou_BDataToTextBox() {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		tbBushouFullName->Text = bdBushouData->lstStrFullName[iSelectedIndex];
		tbBushouRubyYomi->Text = bdBushouData->lstStrRubyYomi[iSelectedIndex];
		tbBushouBornEtc->Text  = bdBushouData->lstStrBornEtc[iSelectedIndex];
		tbBushouRetsuden->Text = bdBushouData->lstStrRetsuden[iSelectedIndex];
	}

	// �e�L�X�g�{�b�N�X�̈ʒu�╝�𒆐g�̃e�L�X�g�ʂɏ]���ĕύX
	void Bushou_TextBoxWidthUpdate() {
		// tbBushouFullName->Width = GetStringMeasure(tbBushouFullName->Text);

		// tbBushouRubyYomi->Left = tbBushouFullName->Right + 8;
		// tbBushouRubyYomi->Width = GetStringMeasure(tbBushouRubyYomi->Text);
		
		// tbBushouBornEtc->Left = tbBushouRubyYomi->Right + 8;
		// tbBushouBornEtc->Width = GetStringMeasure(tbBushouBornEtc->Text);
	}

	// �e�L�X�g�{�b�N�X�́A�u���������v���ύX���ꂽ�B
	void Bushou_tbFullName_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		bdBushouData->lstStrFullName[iSelectedIndex] = tbBushouFullName->Text;
		String^ strLine = String::Format( "{0:0000} - {1}", iSelectedIndex, tbBushouFullName->Text);
		// SelectIndexChange���������Ȃ��悤�ɂ���B
		lbBushouList->SelectedIndexChanged -= gcnew EventHandler(this, &RetsudenEditorForm::Bushou_lbBushouList_SelectedIndexChanged);

		// �������Ȃ��悤�ɂ����̂ŁA�l������������BSelectedIndexChange���\�b�h���������Ȃ��̂ŃG���[���N���Ȃ��B

		lbBushouList->Items[iSelectedIndex] = gcnew String(strLine); // �������X�g�̖��O�̕����X�V

		Bushou_TextBoxWidthUpdate();

		// �X�V�����̂ŁA������߂��B
		// SelectIndexChange����������悤�ɂ���B
		lbBushouList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_lbBushouList_SelectedIndexChanged);
	}

	// �e�L�X�g�{�b�N�X�́A�u�ӂ肪�ȁv���ύX���ꂽ�B
	void Bushou_tbRubyYomi_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		bdBushouData->lstStrRubyYomi[iSelectedIndex] = tbBushouRubyYomi->Text;
		Bushou_TextBoxWidthUpdate();
	}

	// �e�L�X�g�{�b�N�X�́A�u���N���v���ύX���ꂽ�B
	void Bushou_tbBornEtc_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		bdBushouData->lstStrBornEtc[iSelectedIndex]  = tbBushouBornEtc->Text;
		Bushou_TextBoxWidthUpdate();
	}

	// �e�L�X�g�{�b�N�X�́A�u��`�v���ύX���ꂽ�B
	void Bushou_tbRetsuden_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetBushouCBSelectedIndex();

		bdBushouData->lstStrRetsuden[iSelectedIndex] = tbBushouRetsuden->Text;
	}

	// �e�L�X�g�{�b�N�X���狎����
	void Bushou_textBox_Leave(Object^ sender, EventArgs^ e ) {
	}


	// 50�g�ǉ��{�^��
	void Bushou_btnAddBushous_Click(Object^ sender, EventArgs^ e) {
		int iStart = lbBushouList->Items->Count;
		for ( int i = iStart; i < iStart+50; i++ ) {
			String^ bname = String::Format("�o�^{0:0000}", i);
			// ��Ɍ��f�[�^���瑝�₷
			bdBushouData->lstStrFullName->Add( bname );
			bdBushouData->lstStrRubyYomi->Add( "ĳ۸"+i );
			bdBushouData->lstStrBornEtc->Add( "????�`????" );
			bdBushouData->lstStrRetsuden->Add( "������" );
			String^ strLine = String::Format( "{0:0000} - {1}", i, bname);
			lbBushouList->Items->Add( strLine );
		}

	}

	// 50�g�폜�{�^��
	void Bushou_btnDelBushous_Click(Object^ sender, EventArgs^ e) {
		
		int iStart = lbBushouList->Items->Count-1;

		// �I���C���f�b�N�X���폜�\��̏��ɂ�������A�M���M���̂Ƃ���܂őޔ�
		if ( GetBushouCBSelectedIndex() > iStart - 50 ) {
			lbBushouList->SelectedIndex = iStart - 50;
		}

		for ( int i = iStart; i > iStart-50; i-- ) {
			// ��ɕ������X�g����J�b�g
			lbBushouList->Items->RemoveAt(i);

			// �����Ō��f�[�^�����炷
			bdBushouData->lstStrFullName->RemoveAt( i );
			bdBushouData->lstStrRubyYomi->RemoveAt( i );
			bdBushouData->lstStrBornEtc->RemoveAt( i );
			bdBushouData->lstStrRetsuden->RemoveAt( i );
		}
	}


//-----------------------------------------------�ƕ��`�n
private:
	TabPage^ tpKahou;
	KahouData^ bdKahouData;

	TextBox^ tbKahouSearch; // �ƕ󌟍�
	Button^ btnKahouSearch; // �����{�^��
	Button^ btnKahouImport; // �捞�{�^��
	Button^ btnKahouExport; // �o�̓{�^��
	Button^ btnKahouAllSave; // �S�ĕۑ�

	ListBox^ lbKahouList; // �ƕ󃊃X�g�̃R���{�{�b�N�X
	int iLastKahouListSelected;

	Panel^ plKahouRetsuden;	 // ��`���p�̃p�l��
	TextBox^ tbKahouFullName; // �ƕ󐩖��̃e�L�X�g�{�b�N�X
	TextBox^ tbKahouRubyYomi; // �ӂ肪�Ȃ̂��
	TextBox^ tbKahouBornEtc;  // ���N�Ȃǂ̏��
	TextBox^ tbKahouRetsuden; // ��`���


	// �t�H���g
	System::Drawing::Font^ fontKahou;
	ComboBox^ cbFontKahou;	// �t�H���g���X�g

	Button^ btnKahouAdds; // 50�g�ǉ�
	Button^ btnKahouDels; // 50�g�ǉ�


private:

	void tpKahou_Init() {

		tpKahou = gcnew TabPage();
		tpKahou->BackColor = Color::Transparent;


		tpKahou->Text = "�ƕ��`";
		tpKahou->Size = tcRE->ClientSize;

		// �ƕ󌟍�
		tbKahouSearch = gcnew TextBox();
		tbKahouSearch->Width = 210;
		tbKahouSearch->Height = 24;
		tbKahouSearch->Left = 10;
		tbKahouSearch->Top = 10;

		// �����{�^��
		btnKahouSearch = gcnew Button();
		btnKahouSearch->Text = "����";
		btnKahouSearch->Width = 90;
		btnKahouSearch->Height = tbKahouSearch->Height;
		btnKahouSearch->Left = tbKahouSearch->Right + 1;
		btnKahouSearch->Top = tbKahouSearch->Top;

		// �捞�{�^��
		btnKahouImport = gcnew Button();
		btnKahouImport->Text = "�P�Ǝ捞";
		btnKahouImport->Width = 105;
		btnKahouImport->Height = tbKahouSearch->Height;
		btnKahouImport->Left = btnKahouSearch->Right + 45;
		btnKahouImport->Top = tbKahouSearch->Top;

		// �o�̓{�^��
		btnKahouExport = gcnew Button();
		btnKahouExport->Text = "�P�Əo��";
		btnKahouExport->Width = 105;
		btnKahouExport->Height = tbKahouSearch->Height;
		btnKahouExport->Left = btnKahouImport->Right;
		btnKahouExport->Top = tbKahouSearch->Top;

		// �S�ĕۑ�
		btnKahouAllSave = gcnew Button();
		btnKahouAllSave->Text = "�S�ĕۑ�";
		btnKahouAllSave->Width = 105;
		btnKahouAllSave->Height = tbKahouSearch->Height;
		btnKahouAllSave->Left = tpKahou->Right - (btnKahouAllSave->Width+30);
		btnKahouAllSave->Top = tbKahouSearch->Top;

		// �ƕ�m���
		lbKahouList = gcnew ListBox();
		lbKahouList->Width = tbKahouSearch->Width;
		lbKahouList->Left = tbKahouSearch->Left;
		lbKahouList->Top = tbKahouSearch->Bottom + 15;
		lbKahouList->Height = 300;

		// ��`���p�̃p�l��
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

		// �t�H���g���X�g
		cbFontKahou = gcnew ComboBox();
		cbFontKahou->Width = 150;
		cbFontKahou->Left = lbKahouList->Right + 10;
		cbFontKahou->Top = plKahouRetsuden->Bottom + 15;
		cbFontKahou->Height = tbKahouSearch->Height;
		Kahou_GetValidTenshoukiFontList();

		Kahou_cbFont_SelectedIndexChanged(nullptr, nullptr);

		// �ƕ�g50�l�ǉ�
		btnKahouAdds = gcnew Button();
		btnKahouAdds->Text = "50�g�ǉ�";
		btnKahouAdds->Width = 70;
		btnKahouAdds->Height = tbKahouSearch->Height;
		btnKahouAdds->Left = tbKahouSearch->Left;
		btnKahouAdds->Top = lbKahouList->Bottom + 10;

		// �ƕ�g50�l�폜
		btnKahouDels = gcnew Button();
		btnKahouDels->Text = "50�g�폜";
		btnKahouDels->Width = 70;
		btnKahouDels->Height = tbKahouSearch->Height;
		btnKahouDels->Left = btnKahouAdds->Right;
		btnKahouDels->Top = lbKahouList->Bottom + 10;


		// �ƕ��`�̃^�u�ɃR���|�[�l���g�ǉ�
		tpKahou->Controls->Add(tbKahouSearch);
		tpKahou->Controls->Add(btnKahouSearch);
		// tpKahou->Controls->Add(btnKahouImport);
		// tpKahou->Controls->Add(btnKahouExport);
		tpKahou->Controls->Add(btnKahouAllSave);
		tpKahou->Controls->Add(lbKahouList);
		tpKahou->Controls->Add(plKahouRetsuden);
		// tpKahou->Controls->Add(cbFontKahou);
		// tpKahou->Controls->Add(btnKahouAdds); // �ƕ�p�ɂ͂��̃{�^���͕s�v
		// tpKahou->Controls->Add(btnKahouDels); // �ƕ�p�ɂ͂��̃{�^���͕s�v
		
		// �^�u���t�H�[���ɒǉ�
		tcRE->TabPages->Add(tpKahou);

		// �ƕ�f�[�^���쐬���āA���X�g�ɗ�������
		bdKahouData = gcnew KahouData();
		// ���X�g�{�b�N�X�ɑ�������
		for ( int i=0; i<bdKahouData->lstStrFullName->Count; i++ ) {
			String^ strLine = String::Format( "{0:000} - {1}", i, bdKahouData->lstStrFullName[i]);
			lbKahouList->Items->Add( strLine );
		}

		// �ŏ��̂��̂�I�����Ă���
		lbKahouList->SelectedIndex = 0;
		iLastKahouListSelected = 0;

		// �ƕ�f�[�^���e�L�X�g�{�b�N�X
		Kahou_BDataToTextBox();
		Kahou_TextBoxWidthUpdate();
	}

	// �p�l���̔w�i�摜�ݒ�
	void Kahou_SetPanelBackImg() {
		return; 
		// ���̃v���W�F�N�g�̃A�Z���u���̃^�C�v���擾�B
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		// �p�l���̔w�i
		plKahouRetsuden->BackgroundImage = (System::Drawing::Image^)(r->GetObject("retsuden_back"));
		// �w�i�摜�̑傫���Ƀp�l���̑傫�������킹��
		plKahouRetsuden->Size = plKahouRetsuden->BackgroundImage->Size;

	}

	// �e��f�t�h�̃C�x���g�n���h���ݒ�
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

	// �ƕ󌟍��{�^������������
	void Kahou_btnSearch_Click(Object^ sender, EventArgs^ e) {
		if ( tbKahouSearch->Text == "" ) { return; } // �e�L�X�g�G���A������ۂȂ牽�����Ȃ��B

		int iStart = GetKahouCBSelectedIndex() + 1; // ���ݑI�����Ă�����̂̂��玟�̌��

		Regex^ regex = gcnew Regex(tbKahouSearch->Text);

		bool isExist = false;
		// iStart�ȍ~��S���T�[�`���āA
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

	// �ƕ󌟍��e�L�X�g�{�b�N�X�ŃL�[����������
	void Kahou_tbSearch_KeyDown(Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Return ) {
			Kahou_btnSearch_Click(sender, e);
		}
		if (e->KeyCode == Keys::F3 ) {
			Kahou_btnSearch_Click(sender, e);
		}
	}

	// �ƕ�o�̓{�^������������
	void Kahou_btnExport_Click(Object^ sender, EventArgs^ e) {
	}


	// �ƕ���̓{�^������������
	void Kahou_btnImport_Click(Object^ sender, EventArgs^ e) {
	}

	// �S�ĕۑ��{�^������������
	void Kahou_btnAllSave_Click(Object^ sender, EventArgs^ e) {
		bdKahouData->AllSaveToMessageN6P();
	}

	// �t�H���g���X�g�̑I�����ڂ��ύX���ꂽ��
	void Kahou_cbFont_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		// �t�H���g�̐ݒ�
		fontKahou = gcnew System::Drawing::Font((String^)cbFontKahou->Items[cbFontKahou->SelectedIndex], 12, FontStyle::Regular);
		tbKahouFullName->Font = fontKahou;
		tbKahouRubyYomi->Font = fontKahou;
		tbKahouBornEtc->Font  = fontKahou;
		tbKahouRetsuden->Font = fontKahou;
	}

	// �C���X�g�[������Ă���t�H���g�ɂ��������āA�t�H���g���X�g�{�b�N�X�ɒǉ�
	void Kahou_GetValidTenshoukiFontList() {

		//InstalledFontCollection�I�u�W�F�N�g�̎擾
		System::Drawing::Text::InstalledFontCollection^ ifc = gcnew System::Drawing::Text::InstalledFontCollection();

		//�C���X�g�[������Ă��邷�ׂẴt�H���g�t�@�~���A���擾
		cli::array<FontFamily^>^ ffs = ifc->Families;

		//�����ł͌��ƂȂ肦��t�H���g���̂�
		for ( int i=0; i<aryStrFontCandidate->Length; i++) {

			// �t�H���g���X�g
			for each (FontFamily^ ff in ffs) {

				// ���t�H���g���������B
				if (ff->Name == aryStrFontCandidate[i]) {
					// �R���{�{�b�N�X�ɒǉ�
					cbFontKahou->Items->Add( ff->Name );
				}
			}
		}

		// �P�ȏ�t�H���g����������A�ŏ��̂��̂�I�����Ă����B
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
		// �œ_���������Ă���悤�Ȃ�A�Ō�ɑI�����Ă������̂��������B
		int iSelectIndex = GetKahouCBSelectedIndex();
		if ( iSelectIndex == 0xFFFFFFFF ) {
			lbKahouList->SelectedIndex = iLastKahouListSelected;
		} else {
			iLastKahouListSelected = iSelectIndex;
		}
	}


	// �ƕ󃊃X�g��I��ύX����ƁA�摜�̏�̗�`�e��̒l������ւ��
	void Kahou_lbKahouList_SelectedIndexChanged(Object^ sender, EventArgs^ e ) {
		ModifyCBKahouSelectedIndex();
		// �V����index�̃f�[�^���擾�B
		Kahou_BDataToTextBox();

		ModifyCBKahouSelectedIndex();
	}


	// �ƕ�f�[�^���e�L�X�g�{�b�N�X�֓]��
	void Kahou_BDataToTextBox() {
		int iSelectedIndex = GetKahouCBSelectedIndex();
		tbKahouFullName->Text = bdKahouData->lstStrFullName[iSelectedIndex];
		tbKahouRubyYomi->Text = bdKahouData->lstStrRubyYomi[iSelectedIndex];
		tbKahouBornEtc->Text  = bdKahouData->lstStrBornEtc[iSelectedIndex];
		tbKahouRetsuden->Text = bdKahouData->lstStrRetsuden[iSelectedIndex];
	}

	// �e�L�X�g�{�b�N�X�̈ʒu�╝�𒆐g�̃e�L�X�g�ʂɏ]���ĕύX
	void Kahou_TextBoxWidthUpdate() {
		return;

		tbKahouFullName->Width = GetStringMeasure(tbKahouFullName->Text);

		tbKahouRubyYomi->Left = tbKahouFullName->Right + 8;
		tbKahouRubyYomi->Width = GetStringMeasure(tbKahouRubyYomi->Text);
		
		tbKahouBornEtc->Left = tbKahouRubyYomi->Right + 8;
		tbKahouBornEtc->Width = GetStringMeasure(tbKahouBornEtc->Text);
	}

	// �e�L�X�g�{�b�N�X�́A�u�ƕ󐩖��v���ύX���ꂽ�B
	void Kahou_tbFullName_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetKahouCBSelectedIndex();
		bdKahouData->lstStrFullName[iSelectedIndex] = tbKahouFullName->Text;
		String^ strLine = String::Format( "{0:000} - {1}", iSelectedIndex, tbKahouFullName->Text);
		// SelectIndexChange���������Ȃ��悤�ɂ���B
		lbKahouList->SelectedIndexChanged -= gcnew EventHandler(this, &RetsudenEditorForm::Kahou_lbKahouList_SelectedIndexChanged);

		// �������Ȃ��悤�ɂ����̂ŁA�l������������BSelectedIndexChange���\�b�h���������Ȃ��̂ŃG���[���N���Ȃ��B

		lbKahouList->Items[iSelectedIndex] = gcnew String(strLine); // �ƕ󃊃X�g�̖��O�̕����X�V

		Kahou_TextBoxWidthUpdate();

		// �X�V�����̂ŁA������߂��B
		// SelectIndexChange����������悤�ɂ���B
		lbKahouList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_lbKahouList_SelectedIndexChanged);
	}

	// �e�L�X�g�{�b�N�X�́A�u�ӂ肪�ȁv���ύX���ꂽ�B
	void Kahou_tbRubyYomi_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetKahouCBSelectedIndex();
		bdKahouData->lstStrRubyYomi[iSelectedIndex] = tbKahouRubyYomi->Text;
		Kahou_TextBoxWidthUpdate();
	}

	// �e�L�X�g�{�b�N�X�́A�u���N���v���ύX���ꂽ�B
	void Kahou_tbBornEtc_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetKahouCBSelectedIndex();
		bdKahouData->lstStrBornEtc[iSelectedIndex]  = tbKahouBornEtc->Text;
		Kahou_TextBoxWidthUpdate();
	}

	// �e�L�X�g�{�b�N�X�́A�u��`�v���ύX���ꂽ�B
	void Kahou_tbRetsuden_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetKahouCBSelectedIndex();

		bdKahouData->lstStrRetsuden[iSelectedIndex] = tbKahouRetsuden->Text;
	}

	// �e�L�X�g�{�b�N�X���狎����
	void Kahou_textBox_Leave(Object^ sender, EventArgs^ e ) {
	}


	// 50�g�ǉ��{�^��
	void Kahou_btnAddKahous_Click(Object^ sender, EventArgs^ e) {
	}

	// 50�g�폜�{�^��
	void Kahou_btnDelKahous_Click(Object^ sender, EventArgs^ e) {
	}

//-----------------------------------------------���`�n
private:
	TabPage^ tpCastle;
	CastleData^ bdCastleData;

	TextBox^ tbCastleSearch; // �錟��
	Button^ btnCastleSearch; // �����{�^��
	Button^ btnCastleImport; // �捞�{�^��
	Button^ btnCastleExport; // �o�̓{�^��
	Button^ btnCastleAllSave; // �S�ĕۑ�

	TreeView^ tvCastleList; // �郊�X�g�̃c���[�r���[
	int iLastCastleListSelected;
	List<TreeNode^>^ refTvCastleListNodes; // �郊�X�g�̃c���[�r���[���ɂ���m�[�h�̃��t�@�����X

	Panel^ plCastleRetsuden;	 // ��`���p�̃p�l��
	TextBox^ tbCastleFullName; // �鐩���̃e�L�X�g�{�b�N�X
	TextBox^ tbCastleRubyYomi; // �ӂ肪�Ȃ̂��
	TextBox^ tbCastleBornEtc;  // ���N�Ȃǂ̏��
	TextBox^ tbCastleRetsuden; // ��`���


	// �t�H���g
	System::Drawing::Font^ fontCastle;
	ComboBox^ cbFontCastle;	// �t�H���g���X�g

	Button^ btnCastleAdds; // 50�g�ǉ�
	Button^ btnCastleDels; // 50�g�ǉ�


private:

	void tpCastle_Init() {

		tpCastle = gcnew TabPage();
		tpCastle->BackColor = Color::Transparent;

		tpCastle->Text = "���`";
		tpCastle->Size = tcRE->ClientSize;

		// �錟��
		tbCastleSearch = gcnew TextBox();
		tbCastleSearch->Width = 210;
		tbCastleSearch->Height = 24;
		tbCastleSearch->Left = 10;
		tbCastleSearch->Top = 10;

		// �����{�^��
		btnCastleSearch = gcnew Button();
		btnCastleSearch->Text = "����";
		btnCastleSearch->Width = 90;
		btnCastleSearch->Height = tbCastleSearch->Height;
		btnCastleSearch->Left = tbCastleSearch->Right + 1;
		btnCastleSearch->Top = tbCastleSearch->Top;

		// �捞�{�^��
		btnCastleImport = gcnew Button();
		btnCastleImport->Text = "�P�Ǝ捞";
		btnCastleImport->Width = 105;
		btnCastleImport->Height = tbCastleSearch->Height;
		btnCastleImport->Left = btnCastleSearch->Right + 45;
		btnCastleImport->Top = tbCastleSearch->Top;

		// �o�̓{�^��
		btnCastleExport = gcnew Button();
		btnCastleExport->Text = "�P�Əo��";
		btnCastleExport->Width = 105;
		btnCastleExport->Height = tbCastleSearch->Height;
		btnCastleExport->Left = btnCastleImport->Right;
		btnCastleExport->Top = tbCastleSearch->Top;

		// �S�ĕۑ�
		btnCastleAllSave = gcnew Button();
		btnCastleAllSave->Text = "�S�ĕۑ�";
		btnCastleAllSave->Width = 105;
		btnCastleAllSave->Height = tbCastleSearch->Height;
		btnCastleAllSave->Left = tpCastle->Right - (btnCastleAllSave->Width+30);
		btnCastleAllSave->Top = tbCastleSearch->Top;

		// ��m���
		tvCastleList = gcnew TreeView();
		tvCastleList->HideSelection = false;
		tvCastleList->Width = tbCastleSearch->Width;
		tvCastleList->Left = tbCastleSearch->Left;
		tvCastleList->Top = tbCastleSearch->Bottom + 15;
		tvCastleList->Height = 300;

		// ��`���p�̃p�l��
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

		// �t�H���g���X�g
		cbFontCastle = gcnew ComboBox();
		cbFontCastle->Width = 100;
		cbFontCastle->Left = tvCastleList->Right + 10;
		cbFontCastle->Top = plCastleRetsuden->Bottom + 15;
		cbFontCastle->Height = tbCastleSearch->Height;
		Castle_GetValidTenshoukiFontList();

		Castle_cbFont_SelectedIndexChanged(nullptr, nullptr);

		// ��g50�l�ǉ�
		btnCastleAdds = gcnew Button();
		btnCastleAdds->Text = "50�g�ǉ�";
		btnCastleAdds->Width = 70;
		btnCastleAdds->Height = tbCastleSearch->Height;
		btnCastleAdds->Left = tbCastleSearch->Left;
		btnCastleAdds->Top = tvCastleList->Bottom + 10;

		// ��g50�l�폜
		btnCastleDels = gcnew Button();
		btnCastleDels->Text = "50�g�폜";
		btnCastleDels->Width = 70;
		btnCastleDels->Height = tbCastleSearch->Height;
		btnCastleDels->Left = btnCastleAdds->Right;
		btnCastleDels->Top = tvCastleList->Bottom + 10;


		// ���`�̃^�u�ɃR���|�[�l���g�ǉ�
		tpCastle->Controls->Add(tbCastleSearch);
		tpCastle->Controls->Add(btnCastleSearch);
		// tpCastle->Controls->Add(btnCastleImport);
		// tpCastle->Controls->Add(btnCastleExport);
		tpCastle->Controls->Add(btnCastleAllSave);
		tpCastle->Controls->Add(tvCastleList);
		tpCastle->Controls->Add(plCastleRetsuden);
		// tpCastle->Controls->Add(cbFontCastle);
		// tpCastle->Controls->Add(btnCastleAdds); // ��p�ɂ͂��̃{�^���͕s�v
		// tpCastle->Controls->Add(btnCastleDels); // ��p�ɂ͂��̃{�^���͕s�v
		
		// �^�u���t�H�[���ɒǉ�
		tcRE->TabPages->Add(tpCastle);

		// ��f�[�^���쐬���āA���X�g�ɗ�������
		bdCastleData = gcnew CastleData();

		// ���t�@�����X�p�̂��߁B�������₷�����邽�߁B
		refTvCastleListNodes = gcnew List<TreeNode^>();

		// �c���[�r���[�ɗv�f�𑫂�����
		Castle_AddElementTreeView();

		// �ŏ��̂��̂�I�����Ă���
		iLastCastleListSelected = 0;

		// ��f�[�^���e�L�X�g�{�b�N�X
		Castle_BDataToTextBox();
		Castle_TextBoxWidthUpdate();
	}

	// ���c���[�r���[�ɗv�f�𑫂�����
	void Castle_AddElementTreeView() {
		// ���X�g�{�b�N�X�ɑ�������
		for ( int i=0; i<bdCastleData->lstStrFullName->Count; i++ ) {
			String^ strLine = String::Format( "{0:000} - {1}", i, bdCastleData->lstStrFullName[i]);

			TreeNode^ treeNode;
			List<TreeNode^>^ treeNodeNestList = gcnew List<TreeNode^>();

			// �Q�ȏ�̏邪�P�Z�b�g�ɂȂ��Ă���ꍇ�B�T�u�Ƀm�[�h����������
			if ( vCastleInfoListData[i].size() >= 2) { 
				cli::array<TreeNode^>^ treeSubNodeList = gcnew cli::array<TreeNode^>( vCastleInfoListData[i].size()-1 ); 
				for (int j=1; j<(int)vCastleInfoListData[i].size(); j++) {
					TreeNode^ treeNode = gcnew TreeNode( gcnew String(vCastleInfoListData[i][j].szFullName) );
					treeSubNodeList[j-1] = treeNode;

					// ���ʂ̂��߁A�^�O�����Ă���
					List<int>^ pair = gcnew List<int>;
					pair->Add(i);
					pair->Add(j);
					treeNode->Tag = pair;

					treeNodeNestList->Add(treeNode);

				}
				treeNode = gcnew TreeNode(strLine, treeSubNodeList); // �����̃m�[�h���q���Ƃ��Ă�������B
				treeNode->ExpandAll();

			} else {
				treeNode = gcnew TreeNode(strLine);
			}

			// ���ʂ̂��߁A�^�O�����Ă���
			List<int>^ pair = gcnew List<int>;
			pair->Add(i);
			pair->Add(0);
			treeNode->Tag = pair;

			// ���ׂ�����������ԂŌ������₷���悤�ɁA���t�@�����X�p�ɂ������Ă���
			refTvCastleListNodes->Add(treeNode);
			// �l�X�g�������������ő����B
			for ( int j=0; j<treeNodeNestList->Count; j++ ) {
				// ���ׂ�����������ԂŌ������₷���悤�ɁA���t�@�����X�p�ɂ������Ă���
				refTvCastleListNodes->Add(treeNodeNestList[j]);
			}
			// TreeViewer�ɑ����Ă���
			tvCastleList->Nodes->Add( treeNode );

			// ���߂Ă�������A����t�H�[�J�X
			if (i==0) {
				tvCastleList->SelectedNode = treeNode;
			}
		}

	}

	// ���p�l���̔w�i�摜�ݒ�
	void Castle_SetPanelBackImg() {
		return;
		// ���̃v���W�F�N�g�̃A�Z���u���̃^�C�v���擾�B
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		// �p�l���̔w�i
		plCastleRetsuden->BackgroundImage = (System::Drawing::Image^)(r->GetObject("retsuden_back"));
		// �w�i�摜�̑傫���Ƀp�l���̑傫�������킹��
		plCastleRetsuden->Size = plCastleRetsuden->BackgroundImage->Size;

	}

	// ���e��f�t�h�̃C�x���g�n���h���ݒ�
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

	// ���錟���{�^������������
	void Castle_btnSearch_Click(Object^ sender, EventArgs^ e) {

		if ( tbCastleSearch->Text == "" ) { return; } // �e�L�X�g�G���A������ۂȂ牽�����Ȃ��B

		int iStart = GetCastleFlatSelectedIndex() + 1; // ���ݑI�����Ă�����̂̂��玟�̌��

		Regex^ regex = gcnew Regex(tbCastleSearch->Text);

		bool isExist = false;
		// iStart�ȍ~��S���T�[�`���āA
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

	// ���錟���e�L�X�g�{�b�N�X�ŃL�[����������
	void Castle_tbSearch_KeyDown(Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Return ) {
			Castle_btnSearch_Click(sender, e);
		}
		if (e->KeyCode == Keys::F3 ) {
			Castle_btnSearch_Click(sender, e);
		}
	}

	// ���S�ĕۑ��{�^������������
	void Castle_btnAllSave_Click(Object^ sender, EventArgs^ e) {
		bdCastleData->AllSaveToMessageN6P();
	}

	// ���t�H���g���X�g�̑I�����ڂ��ύX���ꂽ��
	void Castle_cbFont_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		// �t�H���g�̐ݒ�
		fontCastle = gcnew System::Drawing::Font((String^)cbFontCastle->Items[cbFontCastle->SelectedIndex], 12, FontStyle::Regular);
		tbCastleFullName->Font = fontCastle;
		tbCastleRubyYomi->Font = fontCastle;
		tbCastleBornEtc->Font  = fontCastle;
		tbCastleRetsuden->Font = fontCastle;
	}

	// ���C���X�g�[������Ă���t�H���g�ɂ��������āA�t�H���g���X�g�{�b�N�X�ɒǉ�
	void Castle_GetValidTenshoukiFontList() {

		//InstalledFontCollection�I�u�W�F�N�g�̎擾
		System::Drawing::Text::InstalledFontCollection^ ifc = gcnew System::Drawing::Text::InstalledFontCollection();

		//�C���X�g�[������Ă��邷�ׂẴt�H���g�t�@�~���A���擾
		cli::array<FontFamily^>^ ffs = ifc->Families;

		//�����ł͌��ƂȂ肦��t�H���g���̂�
		for ( int i=0; i<aryStrFontCandidate->Length; i++) {

			// �t�H���g���X�g
			for each (FontFamily^ ff in ffs) {

				// ���t�H���g���������B
				if (ff->Name == aryStrFontCandidate[i]) {
					// �R���{�{�b�N�X�ɒǉ�
					cbFontCastle->Items->Add( ff->Name );
				}
			}
		}

		// �P�ȏ�t�H���g����������A�ŏ��̂��̂�I�����Ă����B
		if ( cbFontCastle->Items->Count > 0 ) {
			cbFontCastle->SelectedIndex = 0;
		}
	}

	// ��(��������������)�A�I�����Ă���m�[�h��ID�𓾂�B�I�����ĂȂ���΁A0��
	int GetCastleFlatSelectedIndex() {
		
		TreeNode^ iSelectedNode = tvCastleList->SelectedNode;
		for ( int i=0; i < refTvCastleListNodes->Count; i++ ) {
			if ( iSelectedNode == refTvCastleListNodes[i] ) {
				return i;
			}
		}
		return 0;
	}


	// ���c���[�̂ǂꂩ���I�����ꂽ��
	void Castle_tvCastleList_AfterSelect(Object^ sender, TreeViewEventArgs^ e) {
		// �V����index�̃f�[�^���擾�B
		Castle_BDataToTextBox();
	}


	// ����f�[�^���e�L�X�g�{�b�N�X�֓]��
	void Castle_BDataToTextBox() {
		TreeNode^ tnSelectedNode = tvCastleList->SelectedNode;
		List<int>^ pair = (List<int>^)tnSelectedNode->Tag;
		int iMainIndex = pair[0];
		int iSubIndex = pair[1];
		tbCastleFullName->Text = gcnew String(vCastleInfoListData[iMainIndex][iSubIndex].szFullName);
		tbCastleRubyYomi->Text = gcnew String(vCastleInfoListData[iMainIndex][iSubIndex].szRubyYomi);
		tbCastleRetsuden->Text = gcnew String(vCastleRetsudenInfoListData[iMainIndex][iSubIndex].szRetsuden);
	}

	// ���e�L�X�g�{�b�N�X�̈ʒu�╝�𒆐g�̃e�L�X�g�ʂɏ]���ĕύX
	void Castle_TextBoxWidthUpdate() {
		return;

		tbCastleFullName->Width = GetStringMeasure(tbCastleFullName->Text);

		tbCastleRubyYomi->Left = tbCastleFullName->Right + 8;
		tbCastleRubyYomi->Width = GetStringMeasure(tbCastleRubyYomi->Text);
		
		tbCastleBornEtc->Left = tbCastleRubyYomi->Right + 8;
		tbCastleBornEtc->Width = GetStringMeasure(tbCastleBornEtc->Text);
	}

	// ���e�L�X�g�{�b�N�X�́A�u�鐩���v���ύX���ꂽ�B
	void Castle_tbFullName_TextChanged(Object^ sender, EventArgs^ e ) {
		// �I���m�[�h��ID
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

	// ���e�L�X�g�{�b�N�X�́A�u�ӂ肪�ȁv���ύX���ꂽ�B
	void Castle_tbRubyYomi_TextChanged(Object^ sender, EventArgs^ e ) {
		// �I���m�[�h��ID
		TreeNode^ tnSelectedNode = tvCastleList->SelectedNode;
		List<int>^ pair = (List<int>^)tnSelectedNode->Tag;
		int iMainIndex = pair[0];
		int iSubIndex = pair[1];

		string strRubyYomi = DotNetStringToMultiByte( tbCastleRubyYomi->Text );
		strcpy( vCastleInfoListData[iMainIndex][iSubIndex].szRubyYomi, strRubyYomi.c_str() );
		Castle_TextBoxWidthUpdate();
	}


	// ���e�L�X�g�{�b�N�X�́A�u��`�v���ύX���ꂽ�B
	void Castle_tbRetsuden_TextChanged(Object^ sender, EventArgs^ e ) {
		// �I���m�[�h��ID
		TreeNode^ tnSelectedNode = tvCastleList->SelectedNode;
		List<int>^ pair = (List<int>^)tnSelectedNode->Tag;
		int iMainIndex = pair[0];
		int iSubIndex = pair[1];

		string strRetsuden = DotNetStringToMultiByte( tbCastleRetsuden->Text );
		strcpy( vCastleRetsudenInfoListData[iMainIndex][iSubIndex].szRetsuden, strRetsuden.c_str() );
		Castle_TextBoxWidthUpdate();
	}

};
