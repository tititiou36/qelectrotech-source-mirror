/*
	Copyright 2006-2019 The QElectroTech Team
	This file is part of QElectroTech.
	
	QElectroTech is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.
	
	QElectroTech is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with QElectroTech.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef QET_ICONS_H
#define QET_ICONS_H
#include <QIcon>
/**
	This namespace is meant to delare icons used within the QElectroTech
	application.
*/
namespace QET {
	namespace Icons {
		void initIcons();
		
		// using the extern keyword enables access to static objects from outside the
		// namespace
		extern QIcon Add;
		extern QIcon AddFolio;
		extern QIcon Allowed;
		extern QIcon ApplicationExit;
		extern QIcon ArrowLeft;
		extern QIcon ArrowLeftDouble;
		extern QIcon ArrowRight;
		extern QIcon ArrowRightDouble;
		extern QIcon Autoconnect;
		extern QIcon BringForward;
		extern QIcon Cancel;
		extern QIcon Conductor;
		extern QIcon ConductorEdit;
		extern QIcon ConductorSettings;
		extern QIcon Configure;
		extern QIcon ConfigureToolbars;
		extern QIcon IC_CopyFile;
		extern QIcon DefaultConductor;
		extern QIcon DiagramAdd;
		extern QIcon Diagram;
		extern QIcon DiagramBg;
		extern QIcon DiagramDelete;
		extern QIcon DialogCancel;
		extern QIcon DialogInformation;
		extern QIcon DialogOk;
		extern QIcon DocumentClose;
		extern QIcon DocumentExport;
		extern QIcon DocumentImport;
		extern QIcon DocumentNew;
		extern QIcon DocumentOpen;
		extern QIcon DocumentOpenRecent;
		extern QIcon DocumentPrint;
		extern QIcon DocumentPrintFrame;
		extern QIcon DocumentSave;
		extern QIcon DocumentSaveAll;
		extern QIcon DocumentSaveAs;
		extern QIcon DocumentSpreadsheet;
		extern QIcon East;
		extern QIcon EditClear;
		extern QIcon EditClearLocationBar;
		extern QIcon EditCopy;
		extern QIcon EditCut;
		extern QIcon EditDelete;
		extern QIcon EditPaste;
		extern QIcon EditRedo;
		extern QIcon EditRename;
		extern QIcon EditSelectAll;
		extern QIcon EditSelectInvert;
		extern QIcon EditSelectNone;
		extern QIcon EditTableCellMerge;
		extern QIcon EditTableCellSplit;
		extern QIcon EditTableDeleteColumn;
		extern QIcon EditTableDeleteRow;
		extern QIcon EditTableInsertColumnLeft;
		extern QIcon EditTableInsertColumnRight;
		extern QIcon EditTableInsertRowAbove;
		extern QIcon EditTableInsertRowUnder;
		extern QIcon EditText;
		extern QIcon EditUndo;
		extern QIcon Element;
		extern QIcon ElementDelete;
		extern QIcon ElementEdit;
		extern QIcon ElementNew;
		extern QIcon EndLineCircle;
		extern QIcon EndLineDiamond;
		extern QIcon EndLineNone;
		extern QIcon EndLineSimple;
		extern QIcon EndLineTriangle;
		extern QIcon EPS;
		extern QIcon Folder;
		extern QIcon FolderDelete;
		extern QIcon FolderEdit;
		extern QIcon FolderNew;
		extern QIcon FolderOnlyThis;
		extern QIcon FolderOpen;
		extern QIcon FolderProperties;
		extern QIcon FolderShowAll;
		extern QIcon Forbidden;
		extern QIcon FullScreenEnter;
		extern QIcon FullScreenExit;
		extern QIcon GoBottom;
		extern QIcon GoDown;
		extern QIcon GoDownDouble;
		extern QIcon GoTop;
		extern QIcon GoUp;
		extern QIcon GoUpDouble;
		extern QIcon Ground;
		extern QIcon Grid;
		extern QIcon Hide;
		extern QIcon Home;
		extern QIcon HotSpot;
		extern QIcon InsertImage;
		extern QIcon Lower;
		extern QIcon IC_MoveFile;
		extern QIcon Names;
		extern QIcon Neutral;
		extern QIcon NewDiagram;
		extern QIcon NewDiagram;
		extern QIcon NewDiagram;
		extern QIcon North;
		extern QIcon ObjectLocked;
		extern QIcon ObjectRotateRight;
		extern QIcon ObjectUnlocked;
		extern QIcon Orientations;
		extern QIcon PartArc;
		extern QIcon PartCircle;
		extern QIcon PartEllipse;
		extern QIcon PartLine;
		extern QIcon PartPolygon;
		extern QIcon PartRectangle;
		extern QIcon PartSelect;
		extern QIcon PartText;
		extern QIcon PartTextField;
		extern QIcon PDF;
		extern QIcon Phase;
		extern QIcon PrintAllPages;
		extern QIcon Printer;
		extern QIcon PrintLandscape;
		extern QIcon PrintPortrait;
		extern QIcon PrintTwoPages;
		extern QIcon Project;
		extern QIcon ProjectClose;
		extern QIcon ProjectFile;
		extern QIcon ProjectFileGP;
		extern QIcon ProjectNew;
		extern QIcon ProjectProperties;
		extern QIcon Projects;
		extern QIcon QETIcon;
		extern QIcon QETLogo;
		extern QIcon QETOxygenLogo;
		extern QIcon QtLogo;
		extern QIcon Raise;
		extern QIcon Remove;
		extern QIcon Restore;
		extern QIcon RunDxf;
		extern QIcon SendBackward;
		extern QIcon Settings;
		extern QIcon SinglePage;
		extern QIcon South;
		extern QIcon Start;
		extern QIcon TableOfContent;
		extern QIcon Terminal;
		extern QIcon TitleBlock;
		extern QIcon TitleBlockBottom;
		extern QIcon TitleBlockRight;
		extern QIcon TransformRotate;
		extern QIcon UserInformations;
		extern QIcon ViewFitWidth;
		extern QIcon ViewFitWindow;
		extern QIcon ViewMove;
		extern QIcon ViewRefresh;
		extern QIcon West;
		extern QIcon WindowNew;
		extern QIcon XmlTextFile;
		extern QIcon ZoomDraw;
		extern QIcon ZoomFitBest;
		extern QIcon ZoomIn;
		extern QIcon ZoomOriginal;
		extern QIcon ZoomOut;
		extern QIcon adding_image;
		extern QIcon resize_image;
		extern QIcon ar;
		extern QIcon br;
		extern QIcon catalonia;
		extern QIcon cs;
		extern QIcon de;
		extern QIcon da;
		extern QIcon gr;
		extern QIcon en;
		extern QIcon es;
		extern QIcon fr;
		extern QIcon hr;
		extern QIcon hu;
		extern QIcon it;
		extern QIcon pl;
		extern QIcon pt;
		extern QIcon ro;
		extern QIcon ru;
		extern QIcon sl;
		extern QIcon nl;
		extern QIcon be;
		extern QIcon tr;
		extern QIcon translation;
		extern QIcon listDrawings;
		extern QIcon AutoNum;
		extern QIcon TerminalStrip;
		extern QIcon QETManual;
		extern QIcon QETDonate;
		extern QIcon QETDownload;
		extern QIcon QETVideo;
		extern QIcon super;
		extern QIcon sub;
		extern QIcon textGroup;
		extern QIcon ElementMaster;
		extern QIcon ElementSlave;
		extern QIcon FolioXrefComing;
		extern QIcon ElementTerminal;
		extern QIcon ColorBrown;
		extern QIcon ColorGray;
		extern QIcon ColorOrange;
		extern QIcon ColorPurple;
		extern QIcon ColorHTMLPinkPink;
		extern QIcon ColorHTMLPinkLightPink;
		extern QIcon ColorHTMLPinkHotPink;
		extern QIcon ColorHTMLPinkDeepPink;
		extern QIcon ColorHTMLPinkPaleVioletRed;
		extern QIcon ColorHTMLPinkMediumVioletRed;
		extern QIcon ColorHTMLRedLightSalmon;
		extern QIcon ColorHTMLRedSalmon;
		extern QIcon ColorHTMLRedDarkSalmon;
		extern QIcon ColorHTMLRedLightCoral;
		extern QIcon ColorHTMLRedIndianRed;
		extern QIcon ColorHTMLRedCrimson;
		extern QIcon ColorHTMLRedFirebrick;
		extern QIcon ColorHTMLRedDarkRed;
		extern QIcon ColorHTMLRedRed;
		extern QIcon ColorHTMLOrangeOrangeRed;
		extern QIcon ColorHTMLOrangeTomato;
		extern QIcon ColorHTMLOrangeCoral;
		extern QIcon ColorHTMLOrangeDarkOrange;
		extern QIcon ColorHTMLOrangeOrange;
		extern QIcon ColorHTMLYellowYellow;
		extern QIcon ColorHTMLYellowLightYellow;
		extern QIcon ColorHTMLYellowLemonChiffon;
		extern QIcon ColorHTMLYellowLightGoldenrodYellow;
		extern QIcon ColorHTMLYellowPapayaWhip;
		extern QIcon ColorHTMLYellowMoccasin;
		extern QIcon ColorHTMLYellowPeachPuff;
		extern QIcon ColorHTMLYellowPaleGoldenrod;
		extern QIcon ColorHTMLYellowKhaki;
		extern QIcon ColorHTMLYellowDarkKhaki;
		extern QIcon ColorHTMLYellowGold;
		extern QIcon ColorHTMLBrownCornsilk;
		extern QIcon ColorHTMLBrownBlanchedAlmond;
		extern QIcon ColorHTMLBrownBisque;
		extern QIcon ColorHTMLBrownNavajoWhite;
		extern QIcon ColorHTMLBrownWheat;
		extern QIcon ColorHTMLBrownBurlywood;
		extern QIcon ColorHTMLBrownTan;
		extern QIcon ColorHTMLBrownRosyBrown;
		extern QIcon ColorHTMLBrownSandyBrown;
		extern QIcon ColorHTMLBrownGoldenrod;
		extern QIcon ColorHTMLBrownDarkGoldenrod;
		extern QIcon ColorHTMLBrownPeru;
		extern QIcon ColorHTMLBrownChocolate;
		extern QIcon ColorHTMLBrownSaddleBrown;
		extern QIcon ColorHTMLBrownSienna;
		extern QIcon ColorHTMLBrownBrown;
		extern QIcon ColorHTMLBrownMaroon;
		extern QIcon ColorHTMLGreenDarkOliveGreen;
		extern QIcon ColorHTMLGreenOlive;
		extern QIcon ColorHTMLGreenOliveDrab;
		extern QIcon ColorHTMLGreenYellowGreen;
		extern QIcon ColorHTMLGreenLimeGreen;
		extern QIcon ColorHTMLGreenLime;
		extern QIcon ColorHTMLGreenLawnGreen;
		extern QIcon ColorHTMLGreenChartreuse;
		extern QIcon ColorHTMLGreenGreenYellow;
		extern QIcon ColorHTMLGreenSpringGreen;
		extern QIcon ColorHTMLGreenMediumSpringGreen;
		extern QIcon ColorHTMLGreenLightGreen;
		extern QIcon ColorHTMLGreenPaleGreen;
		extern QIcon ColorHTMLGreenDarkSeaGreen;
		extern QIcon ColorHTMLGreenMediumAquamarine;
		extern QIcon ColorHTMLGreenMediumSeaGreen;
		extern QIcon ColorHTMLGreenSeaGreen;
		extern QIcon ColorHTMLGreenForestGreen;
		extern QIcon ColorHTMLGreenGreen;
		extern QIcon ColorHTMLGreenDarkGreen;
		extern QIcon ColorHTMLCyanAqua;
		extern QIcon ColorHTMLCyanCyan;
		extern QIcon ColorHTMLCyanLightCyan;
		extern QIcon ColorHTMLCyanPaleTurquoise;
		extern QIcon ColorHTMLCyanAquamarine;
		extern QIcon ColorHTMLCyanTurquoise;
		extern QIcon ColorHTMLCyanMediumTurquoise;
		extern QIcon ColorHTMLCyanDarkTurquoise;
		extern QIcon ColorHTMLCyanLightSeaGreen;
		extern QIcon ColorHTMLCyanCadetBlue;
		extern QIcon ColorHTMLCyanDarkCyan;
		extern QIcon ColorHTMLCyanTeal;
		extern QIcon ColorHTMLBlueLightSteelBlue;
		extern QIcon ColorHTMLBluePowderBlue;
		extern QIcon ColorHTMLBlueLightBlue;
		extern QIcon ColorHTMLBlueSkyBlue;
		extern QIcon ColorHTMLBlueLightSkyBlue;
		extern QIcon ColorHTMLBlueDeepSkyBlue;
		extern QIcon ColorHTMLBlueDodgerBlue;
		extern QIcon ColorHTMLBlueCornflowerBlue;
		extern QIcon ColorHTMLBlueSteelBlue;
		extern QIcon ColorHTMLBlueRoyalBlue;
		extern QIcon ColorHTMLBlueBlue;
		extern QIcon ColorHTMLBlueMediumBlue;
		extern QIcon ColorHTMLBlueDarkBlue;
		extern QIcon ColorHTMLBlueNavy;
		extern QIcon ColorHTMLBlueMidnightBlue;
		extern QIcon ColorHTMLPurpleLavender;
		extern QIcon ColorHTMLPurpleThistle;
		extern QIcon ColorHTMLPurplePlum;
		extern QIcon ColorHTMLPurpleViolet;
		extern QIcon ColorHTMLPurpleOrchid;
		extern QIcon ColorHTMLPurpleFuchsia;
		extern QIcon ColorHTMLPurpleMagenta;
		extern QIcon ColorHTMLPurpleMediumOrchid;
		extern QIcon ColorHTMLPurpleMediumPurple;
		extern QIcon ColorHTMLPurpleBlueViolet;
		extern QIcon ColorHTMLPurpleDarkViolet;
		extern QIcon ColorHTMLPurpleDarkOrchid;
		extern QIcon ColorHTMLPurpleDarkMagenta;
		extern QIcon ColorHTMLPurplePurple;
		extern QIcon ColorHTMLPurpleIndigo;
		extern QIcon ColorHTMLPurpleDarkSlateBlue;
		extern QIcon ColorHTMLPurpleSlateBlue;
		extern QIcon ColorHTMLPurpleMediumSlateBlue;
		extern QIcon ColorHTMLWhiteWhite;
		extern QIcon ColorHTMLWhiteSnow;
		extern QIcon ColorHTMLWhiteHoneydew;
		extern QIcon ColorHTMLWhiteMintCream;
		extern QIcon ColorHTMLWhiteAzure;
		extern QIcon ColorHTMLWhiteAliceBlue;
		extern QIcon ColorHTMLWhiteGhostWhite;
		extern QIcon ColorHTMLWhiteWhiteSmoke;
		extern QIcon ColorHTMLWhiteSeashell;
		extern QIcon ColorHTMLWhiteBeige;
		extern QIcon ColorHTMLWhiteOldLace;
		extern QIcon ColorHTMLWhiteFloralWhite;
		extern QIcon ColorHTMLWhiteIvory;
		extern QIcon ColorHTMLWhiteAntiqueWhite;
		extern QIcon ColorHTMLWhiteLinen;
		extern QIcon ColorHTMLWhiteLavenderBlush;
		extern QIcon ColorHTMLWhiteMistyRose;
		extern QIcon ColorHTMLGrayGainsboro;
		extern QIcon ColorHTMLGrayLightGray;
		extern QIcon ColorHTMLGraySilver;
		extern QIcon ColorHTMLGrayDarkGray;
		extern QIcon ColorHTMLGrayGray;
		extern QIcon ColorHTMLGrayDimGray;
		extern QIcon ColorHTMLGrayLightSlateGray;
		extern QIcon ColorHTMLGraySlateGray;
		extern QIcon ColorHTMLGrayDarkSlateGray;
		extern QIcon ColorHTMLGrayBlack;
	}
}
#endif
