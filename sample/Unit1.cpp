//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <Registry.hpp>
//#include <uuid/uuid.h>
#include <iostream>
//#include "stdafx.h"
#include <rpc.h>
#include <HTTPApp.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#include "iphlpapi.h"
#pragma comment(lib,"iphlpapi.lib")
TForm1 *Form1;
//using namespace std;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	String mac;

	ULONG				nOutBufLen;
	DWORD				ret;
	BYTE*				pBuff;
	PIP_ADAPTER_INFO	pAdapterInfo;

	nOutBufLen = 0;
	::GetAdaptersInfo(NULL,&nOutBufLen);			//�K�v�o�b�t�@�T�C�Y�擾

	pBuff = new BYTE[nOutBufLen];
	pAdapterInfo = (PIP_ADAPTER_INFO) pBuff;

	ret = ::GetAdaptersInfo(pAdapterInfo,&nOutBufLen);
	if(ret == ERROR_SUCCESS)
	{
		while(1) {
			//MAC�A�h���X��\��
			mac.sprintf(L"%02X-%02X-%02X-%02X-%02X-%02X"
					,pAdapterInfo->Address[0]
					,pAdapterInfo->Address[1]
					,pAdapterInfo->Address[2]
					,pAdapterInfo->Address[3]
					,pAdapterInfo->Address[4]
					,pAdapterInfo->Address[5]);

			Memo1->Lines->Add(pAdapterInfo->Type);
			Memo1->Lines->Add(mac);

			pAdapterInfo = pAdapterInfo->Next;
			if(pAdapterInfo == NULL) break;
		}

	}
	delete	pBuff;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
	String sKeyName = "Software\\DD2\\TEST";// �L�[�̖��O
	TRegistry *Registry = new TRegistry(KEY_ALL_ACCESS);
	try {
		Registry->RootKey = HKEY_CURRENT_USER;
		Registry->OpenKey(sKeyName,true);// ��������true�̂Ƃ��A�Ȃ���΍쐬 false�̂Ƃ��A�Ȃ��Ă��쐬���Ȃ�
		// ��������
		Registry->WriteString("HOGE","TAMA");
		Registry->CloseKey();
	}
	catch(...) {
		ShowMessage("���W�X�g���̏����Ɏ��s���܂���");
	}
	delete Registry;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
	String sKeyName = "Software\\DD2\\TEST";// �L�[�̖��O
	String sValue;// �L�[�̒l
	TRegistry *Registry = new TRegistry(KEY_ALL_ACCESS);
	try {
		Registry->RootKey = HKEY_CURRENT_USER;
		Registry->OpenKey(sKeyName,true);// ��������true�̂Ƃ��A�Ȃ���΍쐬 false�̂Ƃ��A�Ȃ��Ă��쐬���Ȃ�
		// �ǂݍ���
		sValue = Registry->ReadString("HOGE");
		Memo1->Lines->Add(sValue);
		Registry->CloseKey();
	}
	catch(...) {
		ShowMessage("���W�X�g���̏����Ɏ��s���܂���");
	}
	delete Registry;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
//	SYSTEMTIME st;
//	GetSystemTime(&st);
	String sTime = Now().FormatString(L"yyyy-mm-dd-hh-mm-ss-s.ff");
	Memo1->Lines->Add(sTime);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
//	unsigned char *p;
//	UUID uuid;
//
//	::UuidCreate(&uuid );                  // UUID����
//	::UuidToString(&uuid, &p );            // UUID�𕶎���ɂ���
//
//	char str[38];
//	strncpy(str, (char*)p, sizeof(str));   // �����ŗp�ӂ����o�b�t�@�ɃR�s�[
//
//	::RpcStringFree(&p );
//
//	Memo1->Lines->Add(str);

	UUID uuid;
	UuidCreate(&uuid);
	unsigned char *ch = (unsigned char *)&uuid;
	char *szUuid = NULL;
	UuidToStringA(&uuid, (RPC_CSTR *)&szUuid);
	Memo1->Lines->Add(szUuid);
//	printf("%s\n", szUuid);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
	DynamicArray<String> aTmp;
	int iByte   = 0;
	int iPos    = 1;
	int iCnt    = 0;
	int iLineMaxChar = 54;                      //���V�[�g��1�s�ő�o�C�g��
	int iLineMaxChar2 = 38;                      //���V�[�g��1�s�ő�o�C�g��
	int iS_FontSize  = 0;                      //�ʏ�t�H���g�T�C�Y
	int iN_FontSize  = 0;                      //�����߃t�H���g�T�C�Y
	int iN_Font_LineMaxChar = 0;               //�ʏ�t�H���g�T�C�Y�̃��V�[�g��1�s�ő�o�C�g��
	int iMaxChar = 0;
	String aMsg = "";
	//�㕔�R�����g
	iByte   = 0;
	iPos    = 1;
	iCnt    = 0;
	aTmp.Length = 0;
	aMsg = Memo1->Text;
	//�ő啶�����ȏ�̏ꍇ���s����
	for (int j = 1; j <= aMsg.Length(); j++) {
		if (aMsg.SubString(j,2) == "\r\n") {
			j++;
			aTmp.Length++;
			aTmp[iCnt] = aMsg.SubString(iPos,j-iPos);
			iCnt++;
			iPos = j+1;
			iByte = 0;
		} else if (CheckHan(aMsg.SubString(j,1))) {
			//���p����
			iByte = iByte + 1;
		} else if (!CheckHan(aMsg.SubString(j,1))) {
			//�S�p����
			iByte = iByte + 2;
		}
		if (iByte > iLineMaxChar2) {//���s
			aTmp.Length++;
			aTmp[iCnt] = aMsg.SubString(iPos,j-iPos);
			iCnt++;
			iPos = j;
			iByte = 0;
		}
	}
	if (iByte >= 0) {
		aTmp.Length++;
		aTmp[iCnt] = aMsg.SubString(iPos,aMsg.Length());
	}
	for (int i = 0; i < aTmp.Length; i++) {
		Memo2->Lines->Add(aTmp[i]);
	}
}
//---------------------------------------------------------------------------

bool TForm1::CheckHan(String text)
{
	AnsiString ansi = text;
	int a = text.Length();
	int b = ansi.Length();
	if (a == b) return true;
	else        return false;
}


