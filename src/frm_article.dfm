object FrmArticle: TFrmArticle
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Article'
  ClientHeight = 625
  ClientWidth = 552
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  DesignSize = (
    552
    625)
  PixelsPerInch = 96
  TextHeight = 13
  object lbl_deposant: TLabel
    Left = 69
    Top = 12
    Width = 53
    Height = 13
    Caption = 'D'#233'posant :'
  end
  object lbl_reference: TLabel
    Left = 65
    Top = 39
    Width = 57
    Height = 13
    Caption = 'R'#233'f'#233'rence :'
  end
  object lbl_libelle: TLabel
    Left = 86
    Top = 63
    Width = 36
    Height = 13
    Caption = 'Libell'#233' :'
  end
  object lbl_lot: TLabel
    Left = 100
    Top = 89
    Width = 22
    Height = 13
    Caption = 'Lot :'
  end
  object lbl_dlv: TLabel
    Left = 19
    Top = 108
    Width = 103
    Height = 13
    Caption = 'Date limite de vente :'
  end
  object lbl_nbjour: TLabel
    Left = 160
    Top = 108
    Width = 67
    Height = 13
    Caption = 'D'#233'lai (Jours) :'
  end
  object cb_deposant: TComboBox
    Left = 128
    Top = 8
    Width = 145
    Height = 21
    TabOrder = 0
    Text = 'cb_deposant'
  end
  object ed_reference: TEdit
    Left = 128
    Top = 34
    Width = 121
    Height = 21
    TabOrder = 1
    Text = 'ed_reference'
  end
  object ed_libelle: TEdit
    Left = 128
    Top = 60
    Width = 265
    Height = 21
    TabOrder = 2
    Text = 'ed_libelle'
  end
  object chk_lot: TCheckBox
    Left = 128
    Top = 86
    Width = 17
    Height = 17
    TabOrder = 3
  end
  object chk_dlv: TCheckBox
    Left = 128
    Top = 108
    Width = 17
    Height = 17
    TabOrder = 4
    OnClick = chk_dlvClick
  end
  object gb_palette: TGroupBox
    Left = 8
    Top = 131
    Width = 537
    Height = 113
    Caption = 'Palette :'
    TabOrder = 6
    object lbl_p_dimensions: TLabel
      Left = 16
      Top = 39
      Width = 107
      Height = 13
      Caption = 'Dimensions ( LxHxP ) :'
    end
    object lbl_p_quantite: TLabel
      Left = 74
      Top = 64
      Width = 49
      Height = 13
      Caption = 'Quantit'#233' :'
    end
    object lbl_p_poids: TLabel
      Left = 91
      Top = 89
      Width = 32
      Height = 13
      Caption = 'Poids :'
    end
    object lbl_p_x1: TLabel
      Left = 256
      Top = 39
      Width = 6
      Height = 13
      Caption = 'X'
    end
    object lbl_p_x2: TLabel
      Left = 398
      Top = 39
      Width = 6
      Height = 13
      Caption = 'X'
    end
    object lbl_p_libelle: TLabel
      Left = 87
      Top = 18
      Width = 36
      Height = 13
      Alignment = taRightJustify
      Caption = 'Libell'#233' :'
    end
    object ed_p_largeur: TEdit
      Left = 129
      Top = 35
      Width = 121
      Height = 21
      TabOrder = 1
      Text = 'ed_p_largeur'
    end
    object ed_p_hauteur: TEdit
      Left = 271
      Top = 35
      Width = 121
      Height = 21
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      Text = 'ed_p_hauteur'
    end
    object ed_p_profondeur: TEdit
      Left = 410
      Top = 35
      Width = 121
      Height = 21
      TabOrder = 3
      Text = 'ed_p_profondeur'
    end
    object ed_p_quantite: TEdit
      Left = 129
      Top = 60
      Width = 121
      Height = 21
      TabOrder = 4
      Text = 'ed_p_quantite'
    end
    object ed_p_poids: TEdit
      Left = 129
      Top = 87
      Width = 121
      Height = 21
      TabOrder = 5
      Text = 'ed_p_poids'
    end
    object ed_p_libelle: TEdit
      Left = 129
      Top = 14
      Width = 121
      Height = 21
      TabOrder = 0
      Text = 'ed_p_libelle'
    end
  end
  object btn_valider: TButton
    Left = 469
    Top = 592
    Width = 75
    Height = 25
    Action = Action_article_enregistrer
    Anchors = [akRight, akBottom]
    TabOrder = 10
  end
  object btn_annuler: TButton
    Left = 388
    Top = 592
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&Annuler'
    TabOrder = 11
    OnClick = btn_annulerClick
  end
  object gb_contionnement: TGroupBox
    Left = 8
    Top = 245
    Width = 537
    Height = 113
    Caption = 'Conditionnement :'
    TabOrder = 7
    object lbl_c_dimensions: TLabel
      Left = 16
      Top = 39
      Width = 107
      Height = 13
      Caption = 'Dimensions ( LxHxP ) :'
    end
    object lbl_c_quantite: TLabel
      Left = 74
      Top = 64
      Width = 49
      Height = 13
      Caption = 'Quantit'#233' :'
    end
    object lbl_c_poids: TLabel
      Left = 91
      Top = 89
      Width = 32
      Height = 13
      Caption = 'Poids :'
    end
    object lbl_c_x1: TLabel
      Left = 256
      Top = 39
      Width = 6
      Height = 13
      Caption = 'X'
    end
    object lbl_c_x2: TLabel
      Left = 398
      Top = 39
      Width = 6
      Height = 13
      Caption = 'X'
    end
    object lbl_c_libelle: TLabel
      Left = 87
      Top = 18
      Width = 36
      Height = 13
      Alignment = taRightJustify
      Caption = 'Libell'#233' :'
    end
    object ed_c_largeur: TEdit
      Left = 129
      Top = 35
      Width = 121
      Height = 21
      TabOrder = 1
      Text = 'ed_c_largeur'
    end
    object ed_c_hauteur: TEdit
      Left = 271
      Top = 35
      Width = 121
      Height = 21
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      Text = 'ed_c_hauteur'
    end
    object ed_c_profondeur: TEdit
      Left = 410
      Top = 35
      Width = 121
      Height = 21
      TabOrder = 3
      Text = 'ed_c_profondeur'
    end
    object ed_c_quantite: TEdit
      Left = 129
      Top = 60
      Width = 121
      Height = 21
      TabOrder = 4
      Text = 'ed_c_quantite'
    end
    object ed_c_poids: TEdit
      Left = 129
      Top = 87
      Width = 121
      Height = 21
      TabOrder = 5
      Text = 'ed_c_poids'
    end
    object ed_c_libelle: TEdit
      Left = 129
      Top = 14
      Width = 121
      Height = 21
      TabOrder = 0
      Text = 'ed_c_libelle'
    end
  end
  object gb_sousconditionnement: TGroupBox
    Left = 8
    Top = 359
    Width = 537
    Height = 113
    Caption = 'Sous-conditionnement :'
    TabOrder = 8
    object lbl_s_dimensions: TLabel
      Left = 16
      Top = 39
      Width = 107
      Height = 13
      Caption = 'Dimensions ( LxHxP ) :'
    end
    object lbl_s_quantite: TLabel
      Left = 74
      Top = 64
      Width = 49
      Height = 13
      Caption = 'Quantit'#233' :'
    end
    object lbl_s_poids: TLabel
      Left = 91
      Top = 89
      Width = 32
      Height = 13
      Caption = 'Poids :'
    end
    object lbl_s_x1: TLabel
      Left = 256
      Top = 39
      Width = 6
      Height = 13
      Caption = 'X'
    end
    object lbl_s_x2: TLabel
      Left = 398
      Top = 39
      Width = 6
      Height = 13
      Caption = 'X'
    end
    object lbl_s_libelle: TLabel
      Left = 87
      Top = 18
      Width = 36
      Height = 13
      Alignment = taRightJustify
      Caption = 'Libell'#233' :'
    end
    object ed_s_largeur: TEdit
      Left = 129
      Top = 35
      Width = 121
      Height = 21
      TabOrder = 1
      Text = 'ed_s_largeur'
    end
    object ed_s_hauteur: TEdit
      Left = 271
      Top = 35
      Width = 121
      Height = 21
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      Text = 'ed_s_hauteur'
    end
    object ed_s_profondeur: TEdit
      Left = 410
      Top = 35
      Width = 121
      Height = 21
      TabOrder = 3
      Text = 'ed_s_profondeur'
    end
    object ed_s_quantite: TEdit
      Left = 129
      Top = 60
      Width = 121
      Height = 21
      TabOrder = 4
      Text = 'ed_s_quantite'
    end
    object ed_s_poids: TEdit
      Left = 129
      Top = 87
      Width = 121
      Height = 21
      TabOrder = 5
      Text = 'ed_s_poids'
    end
    object ed_s_libelle: TEdit
      Left = 129
      Top = 14
      Width = 121
      Height = 21
      TabOrder = 0
      Text = 'ed_s_libelle'
    end
  end
  object gb_unité: TGroupBox
    Left = 8
    Top = 467
    Width = 537
    Height = 113
    Caption = 'Unit'#233' :'
    TabOrder = 9
    object lbl_u_dimensions: TLabel
      Left = 16
      Top = 39
      Width = 107
      Height = 13
      Caption = 'Dimensions ( LxHxP ) :'
    end
    object lbl_u_quantite: TLabel
      Left = 74
      Top = 64
      Width = 49
      Height = 13
      Caption = 'Quantit'#233' :'
    end
    object lbl_u_poids: TLabel
      Left = 91
      Top = 89
      Width = 32
      Height = 13
      Caption = 'Poids :'
    end
    object lbl_u_x1: TLabel
      Left = 256
      Top = 39
      Width = 6
      Height = 13
      Caption = 'X'
    end
    object lbl_u_x2: TLabel
      Left = 398
      Top = 39
      Width = 6
      Height = 13
      Caption = 'X'
    end
    object lbl_u_libelle: TLabel
      Left = 87
      Top = 18
      Width = 36
      Height = 13
      Alignment = taRightJustify
      Caption = 'Libell'#233' :'
    end
    object ed_u_largeur: TEdit
      Left = 129
      Top = 35
      Width = 121
      Height = 21
      TabOrder = 1
      Text = 'ed_u_largeur'
    end
    object ed_u_hauteur: TEdit
      Left = 271
      Top = 35
      Width = 121
      Height = 21
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      Text = 'ed_u_hauteur'
    end
    object ed_u_profondeur: TEdit
      Left = 410
      Top = 35
      Width = 121
      Height = 21
      TabOrder = 3
      Text = 'ed_u_profondeur'
    end
    object ed_u_quantite: TEdit
      Left = 129
      Top = 60
      Width = 121
      Height = 21
      TabOrder = 4
      Text = 'ed_u_quantite'
    end
    object ed_u_poids: TEdit
      Left = 129
      Top = 87
      Width = 121
      Height = 21
      TabOrder = 5
      Text = 'ed_u_poids'
    end
    object ed_u_libelle: TEdit
      Left = 129
      Top = 14
      Width = 121
      Height = 21
      TabOrder = 0
      Text = 'z'
    end
  end
  object ed_nbjour: TEdit
    Left = 233
    Top = 104
    Width = 72
    Height = 21
    TabOrder = 5
    Text = 'ed_nbjour'
  end
  object ActionList1: TActionList
    Left = 408
    Top = 8
    object Action_article_enregistrer: TAction
      Caption = '&Enregistrer'
      OnExecute = Action_article_enregistrerExecute
    end
  end
end
