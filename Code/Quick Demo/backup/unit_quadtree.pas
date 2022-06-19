unit unit_quadtree;

{$mode ObjFPC}{$H+}

interface

uses
  Classes, SysUtils, graphics, BGRABitmap, BGRABitmapTypes, math;

type

{ tdot }

tdot=record
  fillcolor:tcolor;
  outline:tcolor;
  diameter:integer;
end;

type t2d=record
 x:integer;
 y:integer;
end;

type tsquare=record
 center:t2d;
 height:integer;
 width:integer;
end;

type

{ tsmartdot }

 tsmartdot=class
 private
   location:t2d;
   attr:tdot;
   function get_attr: tdot;
   function get_location: t2d;
   procedure set_attr(AValue: tdot);
   procedure set_location(AValue: t2d);
 public
   constructor create(l:t2d; a:tdot);
   property plocation:t2d read get_location write set_location;
   property pattr:tdot read get_attr  write set_attr;
   procedure paint(c:tcanvas);
   procedure paint(bm:tBGRABitmap;scalefactor:integer);
end;

type

{ tTreeNode }

 tTreeNode=class
  private
   maxitems:integer;
   items:tlist;
   northwest:ttreenode;
   southwest:ttreenode;
   northeast:ttreenode;
   southeast:ttreenode;
   isleaf:boolean;
   dim:tsquare;
   parent:ttreenode;
   procedure divide;
   procedure add_to_child(d:tsmartdot);
   function get_count: integer;
   function get_dim: tsquare;
   function get_isleaf: boolean;
   function get_item(index: integer): tsmartdot;
   function get_maxitems: integer;
   function get_parent: ttreenode;
  public
   constructor create(_parent: ttreenode;_dim:tsquare; _maxitems:integer);
   procedure addItem(item:tsmartdot);
   property pIsLeaf:boolean read get_isleaf;
   property pItems[index:integer]:tsmartdot read get_item;
   property pCount:integer read get_count;
   property pParent:ttreenode read get_parent;
   property pDim:tsquare read get_dim;
   property pMaxitems:integer read get_maxitems;
   procedure paintBorder(c:tcanvas);
   procedure paintBorder(bm:tbgrabitmap; scalefactor:integer);
   procedure paintGraph(bm: TBGRAbitmap; level: integer; xWidth: integer; xPos:integer;csize:integer; vspace:integer; hspace:integer);
   //                   canvas      vertlevel       colwidth         colnr        size of circle  vertical spacer  horizontalspacer
   function get_depth(cd:integer):integer;
   procedure paintDots(c:tcanvas);
   procedure paintDots(bm:tBGRABitmap;scalefactor:integer);
   function TreeCount(known:integer):integer;
end;



implementation

{ tTreeNode }

procedure tTreeNode.divide;
var nw_dim,sw_dim,ne_dim,se_dim:tsquare;
    i:integer;
    d:tsmartdot;
begin
 nw_dim.width:=round(dim.width/2);
 nw_dim.height:=round(dim.height/2);
 nw_dim.center.x:=dim.center.x-round(nw_dim.width/2);
 nw_dim.center.y:=dim.center.y-round(nw_dim.height/2);

 ne_dim.width:=round(dim.width/2);
 ne_dim.height:=round(dim.height/2);
 ne_dim.center.x:=dim.center.x+round(ne_dim.width/2);
 ne_dim.center.y:=dim.center.y-round(ne_dim.height/2);

 sw_dim.width:=round(dim.width/2);
 sw_dim.height:=round(dim.height/2);
 sw_dim.center.x:=dim.center.x-round(sw_dim.width/2);
 sw_dim.center.y:=dim.center.y+round(sw_dim.height/2);

 se_dim.width:=round(dim.width/2);
 se_dim.height:=round(dim.height/2);
 se_dim.center.x:=dim.center.x+round(se_dim.width/2);
 se_dim.center.y:=dim.center.y+round(se_dim.height/2);

 northwest:=ttreenode.create(self,nw_dim,maxitems);
 southwest:=ttreenode.create(self,sw_dim,maxitems);
 northeast:=ttreenode.create(self,ne_dim,maxitems);
 southeast:=ttreenode.create(self,se_dim,maxitems);

 for i:=0 to items.count-1 do begin
   d:=tsmartdot(items.items[i]);
   add_to_child(d);
 end;
 items.Free;
 isLeaf:=false;
end;

procedure tTreeNode.add_to_child(d: tsmartdot);
begin
  if (d.plocation.x < dim.center.x) and (d.plocation.y<dim.center.y) then northwest.addItem(d);
  if (d.plocation.x >= dim.center.x) and (d.plocation.y<dim.center.y) then northeast.addItem(d);
  if (d.plocation.x < dim.center.x) and (d.plocation.y>=dim.center.y) then southwest.addItem(d);
  if (d.plocation.x >= dim.center.x) and (d.plocation.y>=dim.center.y) then southeast.addItem(d);
end;

function tTreeNode.get_count: integer;
begin
 if not isleaf then result:=items.Count
 else result:=0;
end;

function tTreeNode.get_dim: tsquare;
begin
  result:=dim;
end;

function tTreeNode.get_isleaf: boolean;
begin
  result:=isleaf;
end;

function tTreeNode.get_item(index: integer): tsmartdot;
begin
  result:=tsmartdot(items.items[index]);
end;

function tTreeNode.get_maxitems: integer;
begin
  result:=maxitems;
end;

function tTreeNode.get_parent: ttreenode;
begin
  result:=parent;
end;

constructor tTreeNode.create(_parent: ttreenode; _dim: tsquare;
  _maxitems: integer);
begin
  parent:=_parent;
  dim:=_dim;
  if (dim.width<=4) or (dim.height<=4) then maxitems:=integer.MaxValue
  else maxitems:=_maxitems;
  items:=tlist.create;
  isleaf:=true;
end;

procedure tTreeNode.addItem(item: tsmartdot);
begin
  if isleaf then begin
     items.Add(item);
     if items.Count > maxitems then divide;
  end
  else add_to_child(item);

end;

procedure tTreeNode.paintBorder(c: tcanvas);
begin
  c.Pen.Color:=rgbtocolor(0,0,200);
  c.Pen.Style:=pssolid;
  c.Brush.Style:=bsclear;
  c.Pen.Width:=1;
  c.Rectangle((dim.center.x-round(dim.width/2)),(dim.center.y-round(dim.height/2)),(dim.center.x+round(dim.width/2)),(dim.center.y+round(dim.height/2)));

  if not isleaf then begin
    northwest.paintBorder(c);
    southwest.paintBorder(c);
    northeast.paintBorder(c);
    southeast.paintBorder(c);
  end;
end;

procedure tTreeNode.paintBorder(bm: tbgrabitmap; scalefactor:integer);
begin
  bm.canvas.Pen.Color:=rgbtocolor(0,0,200);
  bm.canvas.Pen.Style:=pssolid;
  bm.canvas.Brush.Style:=bsclear;
  bm.canvas.Pen.Width:=1*scalefactor;
  bm.canvas.Rectangle((dim.center.x-round(dim.width/2))*scalefactor,(dim.center.y-round(dim.height/2))*scalefactor,(dim.center.x+round(dim.width/2))*scalefactor,(dim.center.y+round(dim.height/2))*scalefactor);

  if not isleaf then begin
    northwest.paintBorder(bm,scalefactor);
    southwest.paintBorder(bm,scalefactor);
    northeast.paintBorder(bm,scalefactor);
    southeast.paintBorder(bm,scalefactor);
  end;
end;

procedure tTreeNode.paintGraph(bm: TBGRAbitmap; level: integer;
  xWidth: integer; xPos: integer; csize: integer; vspace: integer;
  hspace: integer);
var x,y:integer;
    nextpos:integer;
begin
  //paint self
  y:=round(csize/2)+level*(csize+hspace);
  x:=xPos*round(bm.canvas.Width/(power(4,level)+1));

  nextpos:=round(power(4,level+1)-(xPos*4));


  bm.Canvas.Ellipse(x-round(csize/2),y-round(csize/2),x+round(csize/2),y+round(csize/2));
  if not isleaf then begin
     northwest.paintGraph(bm,level+1,xWidth,nextpos,csize,vspace,hspace);
     southwest.paintGraph(bm,level+1,xWidth,nextpos+1,csize,vspace,hspace);
     southeast.paintGraph(bm,level+1,xWidth,nextpos+2,csize,vspace,hspace);
     northeast.paintGraph(bm,level+1,xWidth,nextpos+3,csize,vspace,hspace);
     //paint conecting lines

  end;
end;

function tTreeNode.get_depth(cd: integer): integer;
var depth:integer;
begin
 result:=cd+1;
  if not isleaf then begin
    depth:=northwest.get_depth(cd+1);
    if depth>result then result:=depth;
    depth:=northeast.get_depth(cd+1);
    if depth>result then result:=depth;
    depth:=southwest.get_depth(cd+1);
    if depth>result then result:=depth;
    depth:=southeast.get_depth(cd+1);
    if depth>result then result:=depth;
  end;
end;

procedure tTreeNode.paintDots(c: tcanvas);
var i:integer;
begin
  if isleaf then begin
    if items.count>0 then begin
      for i:=0 to items.count-1 do begin
        tsmartdot(items.items[i]).paint(c);
      end;
    end;
  end
  else begin
    northwest.paintDots(c);
    southwest.paintDots(c);
    northeast.paintDots(c);
    southeast.paintDots(c);
  end;
end;

procedure tTreeNode.paintDots(bm: tBGRABitmap; scalefactor: integer);
  var i:integer;
begin
  if isleaf then begin
    if items.count>0 then begin
      for i:=0 to items.count-1 do begin
        tsmartdot(items.items[i]).paint(bm,scalefactor);
      end;
    end;
  end
  else begin
    northwest.paintDots(bm,scalefactor);
    southwest.paintDots(bm,scalefactor);
    northeast.paintDots(bm,scalefactor);
    southeast.paintDots(bm,scalefactor);
  end;
end;

function tTreeNode.TreeCount(known: integer): integer;
begin
  if isleaf then begin
    known:=known+items.count;
  end
  else begin
      known:=northwest.TreeCount(known);
      known:=northeast.TreeCount(known);
      known:=southeast.TreeCount(known);
      known:=southwest.TreeCount(known);
  end;
  result:=known;
end;

{ tsmartdot }

function tsmartdot.get_attr: tdot;
begin
  result:=attr;
end;

function tsmartdot.get_location: t2d;
begin
  result:=location;
end;

procedure tsmartdot.set_attr(AValue: tdot);
begin
  attr:=avalue;
end;

procedure tsmartdot.set_location(AValue: t2d);
begin
 location:=avalue;
end;

constructor tsmartdot.create(l: t2d; a: tdot);
begin
 location:=l;
 attr:=a;
end;

procedure tsmartdot.paint(c: tcanvas);
begin
  c.Pen.color:=attr.outline;
  c.Pen.Style:=pssolid;
  c.Pen.Width:=1;
  c.Brush.Color:=attr.fillcolor;
  c.brush.style:=bssolid;
  c.Ellipse(location.x-round(attr.diameter/2),location.y-round(attr.diameter/2),location.x+round(attr.diameter/2),location.y+round(attr.diameter/2));

end;

procedure tsmartdot.paint(bm: tBGRABitmap; scalefactor: integer);
begin
  bm.canvas.Pen.color:=attr.outline;
  bm.canvas.Pen.Style:=pssolid;
  bm.Pen.Width:=1*scalefactor;
  bm.canvas.Brush.Color:=attr.fillcolor;
  bm.canvas.brush.style:=bssolid;
  bm.canvas.Ellipse((location.x-round(attr.diameter/2))*scalefactor,(location.y-round(attr.diameter/2))*scalefactor,(location.x+round(attr.diameter/2))*scalefactor,(location.y+round(attr.diameter/2))*scalefactor);
end;

end.

