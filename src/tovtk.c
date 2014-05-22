#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "array.h"
#include "types.h"
#if defined(LIBXML_TREE_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)

static void xmlAddText(xmlNodePtr node,const char* txt); 

void tovtk(oned_array* scalar_field, const char* fieldname,
		oned_array* vx, oned_array* vy, oned_array* vz,
		twod_array* eltovert,
		const char* filename){

	assert(vx->get_len(vx) == vy->get_len(vy));


	char buf[256];

	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL, unstructuredgrid = NULL, piece = NULL, pointdata = NULL, valsarray = NULL;
	xmlNodePtr celldata = NULL, points = NULL, pointsarray = NULL, cells = NULL, cellarray = NULL, offs = NULL;
	xmlNodePtr celltypes = NULL;

	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "VTKFile");
	xmlNewProp(root_node, BAD_CAST "type", BAD_CAST "UnstructuredGrid");
	xmlNewProp(root_node, BAD_CAST "version",BAD_CAST "0.1");
	xmlNewProp(root_node, BAD_CAST "byte_order", BAD_CAST "LittleEndian");
	xmlDocSetRootElement(doc,root_node);

	long npoints = vx->get_len(vx);
	long nelements = eltovert->get_rows(eltovert);
	long nvert     = eltovert->get_cols(eltovert);

	unstructuredgrid = xmlNewChild(root_node, NULL, BAD_CAST "UnstructuredGrid",NULL);
	piece = xmlNewChild(unstructuredgrid,NULL, BAD_CAST "Piece",NULL);

	sprintf(buf,"%ld",npoints);
	xmlNewProp(piece, BAD_CAST "NumberOfPoints",BAD_CAST buf);

	sprintf(buf,"%ld",npoints);
	xmlNewProp(piece,BAD_CAST "NumberOfCells",BAD_CAST buf);

	pointdata = xmlNewChild(piece,NULL, BAD_CAST "PointData",NULL);
	xmlNewProp(pointdata,BAD_CAST "Scalars",fieldname);


	valsarray = xmlNewChild(pointdata,NULL,BAD_CAST "DataArray",NULL);
	xmlNewProp(valsarray,BAD_CAST "type",BAD_CAST "Float32");
	xmlNewProp(valsarray,BAD_CAST "Name",BAD_CAST fieldname);
	xmlNewProp(valsarray,BAD_CAST "format",BAD_CAST "ascii");
	xmlAddText(valsarray,"\n");
	Index i;
	for(i=0;i<npoints;i++){
		Real val = scalar_field->get_val(scalar_field,i);
		sprintf(buf,"%f\n",val);
		xmlAddText(valsarray,BAD_CAST buf);
	}

	celldata = xmlNewChild(piece,NULL,BAD_CAST "CellData",NULL);
	points = xmlNewChild(piece,NULL,BAD_CAST "Points",NULL);
	pointsarray = xmlNewChild(points,NULL,BAD_CAST "DataArray",NULL);
	xmlNewProp(pointsarray,BAD_CAST "type",BAD_CAST "Float32");
	xmlNewProp(pointsarray,BAD_CAST "format",BAD_CAST "ascii");
	xmlNewProp(pointsarray,BAD_CAST "NumberOfComponents",BAD_CAST "3");
	xmlAddText(pointsarray, BAD_CAST "\n");
	for(i=0;i<npoints;i++){
		Real x = vx->get_val(vx,i);
		Real y = vy->get_val(vy,i);
		Real z = vz->get_val(vz,i);
		sprintf(buf,"%f %f %f\n",x,y,z);
		xmlAddText(pointsarray,BAD_CAST buf);
	}

	cells = xmlNewChild(piece,NULL,BAD_CAST "Cells",NULL);
	cellarray = xmlNewChild(cells,NULL,BAD_CAST "DataArray",NULL);
	xmlNewProp(cellarray,BAD_CAST "type", BAD_CAST "Int32");
	xmlNewProp(cellarray,BAD_CAST "Name",BAD_CAST "connectivity");
	xmlNewProp(cellarray,BAD_CAST "format",BAD_CAST "ascii");

	xmlAddText(cellarray,"\n");
	Index k,v;
	for(k=0;k<nelements;k++){
		for(v=0;v<nvert;v++){
			Index v1 = eltovert->get_val(eltovert,k,v);
			sprintf(buf,"%ld ",v1);
			xmlAddText(cellarray,BAD_CAST buf);
		}
		xmlAddText(cellarray,"\n");
	}

	offs = xmlNewChild(cells,NULL,BAD_CAST "DataArray",NULL);
	xmlNewProp(offs,BAD_CAST "type",BAD_CAST "Int32");
	xmlNewProp(offs,BAD_CAST "Name",BAD_CAST "offsets");
	xmlNewProp(offs,BAD_CAST "format",BAD_CAST "ascii");
	Index offset=nvert;
	for(k=0;k<nelements;k++){
		sprintf(buf,"%ld ",offset);
		xmlAddText(offs,BAD_CAST buf);
		offset=offset+nvert;
	}

	Index eltype;
	if(nvert==3)
		eltype=5;
	else
		eltype=10;

	celltypes = xmlNewChild(cells,NULL,BAD_CAST "DataArray",NULL);
	xmlNewProp(celltypes,BAD_CAST "type",BAD_CAST "UInt8");
	xmlNewProp(celltypes,BAD_CAST "Name",BAD_CAST "types");
	xmlNewProp(celltypes,BAD_CAST "format",BAD_CAST "ascii");
	xmlAddText(celltypes,"\n");
	sprintf(buf,"%ld ",eltype);
	for(k=0;k<nelements;k++){
		xmlAddText(celltypes,BAD_CAST buf);
	}

	xmlSaveFormatFileEnc(BAD_CAST filename,doc,BAD_CAST "UTF-8", 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();


}


static void xmlAddText(xmlNodePtr node,const char* txt){
	xmlAddChild(node,xmlNewText(BAD_CAST txt));
}
#else
#warning libxml tree support not compiled.
#endif
