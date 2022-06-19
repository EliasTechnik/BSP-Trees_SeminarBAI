unit unitmain;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, Menus, ExtCtrls,
  ComCtrls, unit_quadtree, BGRABitmap, BGRABitmapTypes, math;

type

  { Tformmain }

  Tformmain = class(TForm)
    mi_fhd_169: TMenuItem;
    mi_hd_169: TMenuItem;
    mi_hd_11: TMenuItem;
    mi_view: TMenuItem;
    mi_show_tree: TMenuItem;
    mi_file: TMenuItem;
    mi_save_picture: TMenuItem;
    mi_mode: TMenuItem;
    mi_single: TMenuItem;
    mi_group: TMenuItem;
    mi_activate_pen: TMenuItem;
    mi_canvas: TMenuItem;
    mi_clear: TMenuItem;
    mm: TMainMenu;
    mi_tool: TMenuItem;
    mi_activate_brush: TMenuItem;
    parea: TPaintBox;
    sb: TStatusBar;
    procedure FormCreate(Sender: TObject);
    procedure mi_activate_brushClick(Sender: TObject);
    procedure mi_activate_penClick(Sender: TObject);
    procedure mi_clearClick(Sender: TObject);
    procedure mi_fhd_169Click(Sender: TObject);
    procedure mi_groupClick(Sender: TObject);
    procedure mi_hd_11Click(Sender: TObject);
    procedure mi_hd_169Click(Sender: TObject);
    procedure mi_save_pictureClick(Sender: TObject);
    procedure mi_show_treeClick(Sender: TObject);
    procedure mi_singleClick(Sender: TObject);
    procedure pareaMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure pareaMouseMove(Sender: TObject; Shift: TShiftState; X, Y: Integer
      );
    procedure pareaMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
  private

  public
    mouse_x:integer;
    mouse_y:integer;
    doPaint:boolean;
    def_dot:tdot;
    t:ttreenode;
    maxItems:integer;
    procedure paint_dot(d:tdot; x:integer; y:integer);
    procedure paint_dot_group(d:tdot;count:integer;spread_factor:double;x:integer; y:integer);
    procedure paint;
    procedure clear;
    procedure paintTree;
    procedure updateStats;
    procedure updateviz(full:boolean =false);
    procedure rescale;
  end;

var
  formmain: Tformmain;

implementation

{$R *.lfm}

{ Tformmain }



procedure Tformmain.FormCreate(Sender: TObject);
begin
 doPaint:=false;
 def_dot.fillcolor:=rgbtocolor(200,0,0);
 def_dot.outline:=rgbtocolor(0,0,0);
 def_dot.diameter:=8;
 maxitems:=4;
 parea.Canvas.AntialiasingMode:=amOn;
 randomize();
 rescale;
end;

procedure Tformmain.mi_activate_brushClick(Sender: TObject);
begin
  mi_activate_brush.Checked:=not mi_activate_brush.Checked;
  if mi_activate_pen.checked then mi_activate_pen.checked:=false;
end;

procedure Tformmain.mi_activate_penClick(Sender: TObject);
begin
  mi_activate_pen.Checked:=not mi_activate_pen.Checked;
  if mi_activate_brush.checked then mi_activate_brush.checked:=false;
end;

procedure Tformmain.mi_clearClick(Sender: TObject);
begin
  clear;
end;

procedure Tformmain.mi_fhd_169Click(Sender: TObject);
begin
  mi_fhd_169.checked:= not mi_fhd_169.checked;
  if mi_fhd_169.checked then begin
     mi_hd_169.checked:=false;
     mi_hd_11.checked:=false;
  end;
  rescale;
end;

procedure Tformmain.mi_groupClick(Sender: TObject);
begin
  mi_group.Checked:=not mi_group.Checked;
  if mi_single.checked then mi_single.Checked:=false;
end;

procedure Tformmain.mi_hd_11Click(Sender: TObject);
begin
  mi_hd_11.checked:= not mi_hd_11.checked;
  if mi_hd_11.checked then begin
     mi_hd_169.checked:=false;
     mi_fhd_169.checked:=false;
  end;
  rescale;
end;

procedure Tformmain.mi_hd_169Click(Sender: TObject);
begin
  mi_hd_169.checked:= not mi_hd_169.checked;
  if mi_hd_169.checked then begin
     mi_hd_11.checked:=false;
     mi_fhd_169.checked:=false;
  end;
  rescale;
end;

procedure Tformmain.mi_save_pictureClick(Sender: TObject);
var bm:tbgrabitmap;
   x,y:integer;
begin
  bm:=tbgrabitmap.Create(parea.Width*10,parea.Height*10);
  t.paintDots(bm,10);
  if mi_show_tree.checked then t.paintBorder(bm,10);
  bm.SaveToFile('picture_out.png');
end;

procedure Tformmain.mi_show_treeClick(Sender: TObject);
begin
  mi_show_tree.checked := not mi_show_tree.checked;
  updateviz(true);
end;

procedure Tformmain.mi_singleClick(Sender: TObject);
begin
  if mi_group.Checked then mi_group.Checked:=false;
  mi_single.checked:= not mi_single.Checked;
end;

procedure Tformmain.pareaMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  doPaint:=true;
  paint;
end;

procedure Tformmain.pareaMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  mouse_x:=x;
  mouse_y:=y;
  paint;
end;

procedure Tformmain.pareaMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
   doPaint:=false;
end;

procedure Tformmain.paint_dot(d: tdot; x:integer; y:integer);
var sd:tsmartdot;
    l:t2d;
begin
  l.x:=x;
  l.y:=y;
  sd:=tsmartdot.create(l,d);
  t.addItem(sd);
  updateViz;
  updateStats;
end;

procedure Tformmain.paint_dot_group(d: tdot; count: integer;
  spread_factor: double; x: integer; y: integer);
var i,xc,yc:integer;
begin
  for i:=0 to count-1 do begin
    xc:=x+round((random()-0.5)*2*spread_factor);
    yc:=y+round((random()-0.5)*2*spread_factor);
    paint_dot(d,xc,yc);
  end;

end;

procedure Tformmain.paint;
begin
  if doPaint then begin
      if mi_single.checked then begin
        paint_dot(def_dot,mouse_x,mouse_y);
      end;
      if mi_group.checked then begin
        paint_dot_group(def_dot,7,75,mouse_x,mouse_y);
      end;
    if mi_activate_pen.Checked then begin
     doPaint:=false;
    end;
  end;
end;

procedure Tformmain.clear;
var d:tsquare;
begin
  d.width:=parea.Width;
  d.height:=parea.Height;
  d.center.y:=round(parea.Height/2);
  d.center.x:=round(parea.Width/2);
  if assigned(t) then t.Free;
  t:=ttreenode.create(nil,d,maxitems);
  updateviz(true);
end;

procedure Tformmain.paintTree;
begin
  if assigned(t) then t.paintBorder(parea.Canvas);
  parea.canvas.Refresh;
end;

procedure Tformmain.updateStats;
begin
  sb.Panels.Items[0].Text:='Max. Depth: '+inttostr(t.get_depth(0));
  sb.Panels.Items[1].Text:='Total Items: '+inttostr(t.TreeCount(0));
end;

procedure Tformmain.updateviz(full: boolean);
begin
  if full then begin
    parea.Canvas.Brush.Color:=RGBToColor(255,255,255);
    parea.Canvas.Brush.Style:=bssolid;
    parea.Canvas.FillRect(0,0,parea.Width,parea.height);
  end;
  t.paintDots(parea.canvas);
  if mi_show_tree.checked then paintTree;

  parea.Canvas.refresh;
end;

procedure Tformmain.rescale;
begin
  if mi_hd_11.checked then begin
    formmain.Height:=1024+mm.Height+sb.Height;
    formmain.Width:=1024;
  end;
  if mi_hd_169.checked then begin
    formmain.Height:=768+mm.Height+sb.Height;
    formmain.Width:=1366;
  end;
  if mi_fhd_169.checked then begin
    formmain.Height:=1080+mm.Height+sb.Height;
    formmain.Width:=1920;
  end;
  clear;
end;

end.

