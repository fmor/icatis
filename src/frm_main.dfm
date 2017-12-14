object FrmMain: TFrmMain
  Left = 0
  Top = 0
  Caption = 'FrmMain'
  ClientHeight = 243
  ClientWidth = 542
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  DesignSize = (
    542
    243)
  PixelsPerInch = 96
  TextHeight = 13
  object btn_articles: TButton
    Left = 24
    Top = 16
    Width = 169
    Height = 25
    Caption = '&Articles'
    TabOrder = 0
    OnClick = btn_articlesClick
  end
  object btn_stock: TButton
    Left = 24
    Top = 56
    Width = 169
    Height = 25
    Caption = '&Stock'
    TabOrder = 1
    OnClick = btn_stockClick
  end
  object sb: TStatusBar
    Left = 0
    Top = 224
    Width = 542
    Height = 19
    Panels = <>
  end
  object btn_quitter: TButton
    Left = 392
    Top = 193
    Width = 142
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Se d'#233'connecter && &Quitter'
    TabOrder = 3
    OnClick = btn_quitterClick
  end
end
