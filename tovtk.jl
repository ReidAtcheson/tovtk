using LightXML

function tovtk(VX,VY,EToV,vals,valsname)

    xdoc = XMLDocument();

    xroot = create_root(xdoc, "VTKFile");
    set_attribute(xroot,"type","UnstructuredGrid");
    set_attribute(xroot,"version","0.1");
    set_attribute(xroot,"byte_order","LittleEndian");
   #set_attribute(xroot,"compressor","vtkZLibDataCompressor");


    nPoints = length(VX);
    (K,nFaces) = size(EToV);

    unstructgrid = new_child(xroot, "UnstructuredGrid");

    piece        = new_child(unstructgrid,"Piece");
    set_attribute(piece,"NumberOfPoints",@sprintf("%d",nPoints));
    set_attribute(piece,"NumberOfCells",@sprintf("%d",K));

    pointdata = new_child(piece,"PointData");
    set_attribute(pointdata,"Scalars",valsname);

    valsarray = new_child(pointdata,"DataArray");
    set_attribute(valsarray,"type","Float32");
    set_attribute(valsarray,"Name",valsname);
    set_attribute(valsarray,"format","ascii");
    add_text(valsarray,@sprintf("\n"));
    for i = 1 : nPoints 
        #v1=EToV[k,1];v2=EToV[k,2];v3=EToV[k,3];
        add_text(valsarray,@sprintf("%f\n",vals[i]));
    end

    celldata = new_child(piece,"CellData");

    points = new_child(piece,"Points");
    pointsarray = new_child(points,"DataArray");
    set_attribute(pointsarray,"type","Float32");
    set_attribute(pointsarray,"format","ascii");
    set_attribute(pointsarray,"NumberOfComponents","3");
    add_text(pointsarray,@sprintf("\n"));
    for i = 1 : nPoints
        add_text(pointsarray,@sprintf("%f %f %f\n",VX[i],VY[i],0.0));
    end

    cells = new_child(piece,"Cells");

    cellarray = new_child(cells,"DataArray");
    set_attribute(cellarray,"type","Int32");
    set_attribute(cellarray,"Name","connectivity");
    set_attribute(cellarray,"format","ascii");
    add_text(cellarray,@sprintf("\n"));
    for k = 1 : K
        add_text(cellarray,@sprintf("%d %d %d\n",
        EToV[k,1]-1,EToV[k,2]-1,EToV[k,3]-1));
    end

    offs = new_child(cells,"DataArray");
    set_attribute(offs,"type","Int32");
    set_attribute(offs,"Name","offsets");
    set_attribute(offs,"format","ascii");
    offset=3;
    add_text(offs,@sprintf("\n"));
    for k = 1 : K
        add_text(offs,@sprintf("%d ",offset));
        offset=offset+3;
    end


    celltypes = new_child(cells,"DataArray");
    set_attribute(celltypes,"type","UInt8");
    set_attribute(celltypes,"Name","types");
    set_attribute(celltypes,"format","ascii");
    offset=3;
    add_text(celltypes,@sprintf("\n"));
    for k = 1 : K
        add_text(celltypes,@sprintf("%d ",5));
    end




 




    save_file(xdoc, "test.vtu");


end
