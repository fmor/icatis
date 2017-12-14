object FrmGestionArticles: TFrmGestionArticles
  Left = 0
  Top = 0
  Anchors = [akRight, akBottom]
  Caption = 'Gestion des articles'
  ClientHeight = 636
  ClientWidth = 973
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    973
    636)
  PixelsPerInch = 96
  TextHeight = 13
  object lbl_recherche: TLabel
    Left = 192
    Top = 12
    Width = 58
    Height = 13
    Caption = 'Recherche :'
  end
  object btn_rechercher: TButton
    Left = 890
    Top = 33
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = '&Rechercher'
    TabOrder = 0
    OnClick = btn_rechercherClick
  end
  object btn_ajouter: TButton
    Left = 192
    Top = 583
    Width = 75
    Height = 25
    Action = action_articles_ajouter
    Anchors = [akLeft, akBottom]
    TabOrder = 2
  end
  object btn_supprimer: TButton
    Left = 354
    Top = 583
    Width = 75
    Height = 25
    Action = action_articles_supprimer
    Anchors = [akLeft, akBottom]
    TabOrder = 3
  end
  object btn_fermer: TButton
    Left = 895
    Top = 583
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&Fermer'
    TabOrder = 4
    OnClick = btn_fermerClick
  end
  object sg_articles: TStringGrid
    Left = 192
    Top = 64
    Width = 773
    Height = 513
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 1
    OnDblClick = action_article_modifierExecute
    OnDrawCell = sg_articlesDrawCell
    OnKeyDown = sg_articlesKeyDown
    OnSelectCell = sg_articlesSelectCell
    RowHeights = (
      24
      24
      24
      24
      24)
  end
  object sb_form: TStatusBar
    AlignWithMargins = True
    Left = 3
    Top = 614
    Width = 967
    Height = 19
    Panels = <>
  end
  object cb_recherche_auto: TCheckBox
    Left = 192
    Top = 37
    Width = 97
    Height = 17
    Caption = 'Recherche auto'
    TabOrder = 6
  end
  object btn_effacer: TButton
    Left = 809
    Top = 33
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = '&Effacer'
    TabOrder = 7
    OnClick = btn_effacerClick
  end
  object cb_reference: TCheckBox
    Left = 447
    Top = 35
    Width = 65
    Height = 17
    Anchors = [akLeft, akTop, akRight]
    Caption = 'R'#233'f'#233'rence'
    TabOrder = 8
    OnClick = cb_onclick
  end
  object cb_libelle: TCheckBox
    Left = 518
    Top = 35
    Width = 65
    Height = 17
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Libell'#233
    TabOrder = 9
    OnClick = cb_onclick
  end
  object ed_recherche: TEdit
    Left = 256
    Top = 8
    Width = 709
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 10
    Text = 'ed_recherche'
    OnChange = ed_rechercheChange
    OnKeyDown = ed_rechercheKeyDown
  end
  object lb_deposant: TListBox
    Left = 3
    Top = 8
    Width = 183
    Height = 600
    Anchors = [akLeft, akTop, akBottom]
    ItemHeight = 13
    TabOrder = 11
    OnClick = lb_deposantClick
  end
  object Button1: TButton
    Left = 273
    Top = 583
    Width = 75
    Height = 25
    Action = action_article_modifier
    Anchors = [akLeft, akBottom]
    TabOrder = 12
  end
  object ActionList1: TActionList
    Left = 744
    Top = 24
    object action_articles_supprimer: TAction
      Category = 'Articles'
      Caption = '&Supprimer'
      OnExecute = action_articles_supprimerExecute
    end
    object action_articles_ajouter: TAction
      Category = 'Articles'
      Caption = '&Ajouter'
      OnExecute = action_articles_ajouterExecute
    end
    object action_article_modifier: TAction
      Category = 'Articles'
      Caption = '&Modifier'
      OnExecute = action_article_modifierExecute
    end
  end
  object PopupMenu_Articles: TPopupMenu
    Left = 680
    Top = 24
    object Ouvrir1: TMenuItem
      Action = action_article_modifier
    end
    object Supprimer1: TMenuItem
      Action = action_articles_supprimer
    end
    object Crer1: TMenuItem
      Action = action_articles_ajouter
    end
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 776
    Top = 584
  end
end
