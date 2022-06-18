unit unit_quadtree;

{$mode ObjFPC}{$H+}

interface

uses
  Classes, SysUtils, graphics, BGRABitmap, BGRABitmapTypes;

type

{ tdot }

tdot=record
  color:tcolor;
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
   procedure paintGraph(c: tcanvas; level: integer; xOffset: integer; csize:integer; vspace:integer; hspace:integer);
   function get_depth(cd:integer):integer;
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

procedure tTreeNode.paintGraph(c: tcanvas; level: integer; xOffset: integer; csize:integer; vspace:integer; hspace:integer);
var x,y:integer;
begin
  //paint self
  y:=round(csize/2)+level*(csize+hspace);
  x:=round(c.Width/2)+xoffset;
  c.Ellipse(x-round(csize/2),y-round(csize/2),x+round(csize/2),y+round(csize/2));
  if not isleaf then begin
     northwest.paintGraph(c,level+1,xOffset-(4*(csize+hspace));
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

end.

