object FrmStock: TFrmStock
  Left = 0
  Top = 0
  Caption = 'Stock'
  ClientHeight = 616
  ClientWidth = 1071
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    1071
    616)
  PixelsPerInch = 96
  TextHeight = 13
  object lbl_recherche: TLabel
    Left = 199
    Top = 10
    Width = 58
    Height = 13
    Caption = 'Recherche :'
  end
  object sb_form: TStatusBar
    Left = 0
    Top = 597
    Width = 1071
    Height = 19
    Panels = <>
  end
  object ed_recherche: TEdit
    Left = 263
    Top = 6
    Width = 800
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 2
    Text = 'ed_recherche'
    OnChange = ed_rechercheChange
    OnKeyDown = ed_rechercheKeyDown
  end
  object btn_rechercher: TButton
    Left = 988
    Top = 29
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = '&Rechercher'
    TabOrder = 3
    OnClick = btn_rechercherClick
  end
  object sg_stocklines: TStringGrid
    Left = 199
    Top = 56
    Width = 864
    Height = 505
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 4
    OnDrawCell = sg_stocklinesDrawCell
  end
  object tv_filtre: TTreeView
    Left = 8
    Top = 8
    Width = 185
    Height = 583
    Anchors = [akLeft, akTop, akBottom]
    Indent = 19
    TabOrder = 1
    OnChange = tv_filtreChange
  end
  object btn_effacer: TButton
    Left = 907
    Top = 29
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = '&Effacer'
    TabOrder = 5
    OnClick = btn_effacerClick
  end
  object cb_recherche_auto: TCheckBox
    Left = 199
    Top = 33
    Width = 97
    Height = 17
    Caption = 'Recherche auto'
    TabOrder = 6
  end
  object cb_reference: TCheckBox
    Left = 480
    Top = 33
    Width = 71
    Height = 17
    Caption = 'R'#233'f'#233'rence'
    TabOrder = 7
  end
  object cb_adresse: TCheckBox
    Left = 615
    Top = 33
    Width = 71
    Height = 17
    Caption = 'Adresse'
    TabOrder = 9
  end
  object cb_libelle: TCheckBox
    Left = 557
    Top = 33
    Width = 60
    Height = 17
    Caption = 'Libell'#233
    TabOrder = 8
  end
  object btn_fermer: TButton
    Left = 988
    Top = 567
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&Fermer'
    TabOrder = 10
    OnClick = btn_fermerClick
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 296
    Top = 248
  end
end
