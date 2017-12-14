object FrmLogin: TFrmLogin
  Left = 0
  Top = 0
  Caption = 'Login'
  ClientHeight = 219
  ClientWidth = 375
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object lbl_login: TLabel
    Left = 48
    Top = 83
    Width = 55
    Height = 13
    Caption = 'Utilisateur :'
  end
  object lbl_password: TLabel
    Left = 32
    Top = 112
    Width = 71
    Height = 13
    Caption = 'Mot de passe :'
  end
  object lbl_bdd: TLabel
    Left = 14
    Top = 136
    Width = 89
    Height = 13
    Caption = 'Base de donn'#233'es :'
  end
  object lbl_title: TLabel
    Left = 8
    Top = 9
    Width = 97
    Height = 36
    Caption = 'caption'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -30
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object ed_username: TEdit
    Left = 111
    Top = 80
    Width = 178
    Height = 21
    TabOrder = 0
    Text = 'ed_username'
  end
  object ed_password: TEdit
    Left = 111
    Top = 107
    Width = 178
    Height = 21
    TabOrder = 1
    Text = 'ed_password'
  end
  object ed_database: TEdit
    Left = 111
    Top = 133
    Width = 178
    Height = 21
    TabOrder = 2
    Text = 'ed_database'
  end
  object btn_select_bdd: TButton
    Left = 295
    Top = 131
    Width = 75
    Height = 25
    Caption = 'Choisir'
    TabOrder = 3
    OnClick = btn_select_bddClick
  end
  object btn_login: TButton
    Left = 295
    Top = 184
    Width = 75
    Height = 25
    Caption = '&Se connecter'
    TabOrder = 4
    OnClick = btn_loginClick
  end
  object btn_quit: TButton
    Left = 214
    Top = 184
    Width = 75
    Height = 25
    Caption = '&Quitter'
    TabOrder = 5
    OnClick = btn_quitClick
  end
  object OpenDialog1: TOpenDialog
    Left = 264
    Top = 8
  end
end
