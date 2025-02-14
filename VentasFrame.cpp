#include "VentasFrame.h"
#include "AddVenta.h"
#include "string_conv.h"
#include <wx/msgdlg.h>

VentasFrame::VentasFrame(wxWindow *parent, Sistema *m_sistema) 
	: BaseVentasFrame(parent), sistema(m_sistema) {
	
	ActualizarGridVentas();
	gridVentas->ClearSelection();
}

VentasFrame::~VentasFrame() {}

/// -- Actualizar GRID VENTAS
void VentasFrame::ActualizarGridVentas(){
	gridVentas->ClearSelection();
	if(gridVentas->GetNumberRows() != 0){
		gridVentas->DeleteRows(0,gridVentas->GetNumberRows());
	}
	
	for(int i=0; i<sistema->GetVentasSize(); i++){
		Venta venta = sistema->GetVenta(i);
		gridVentas->AppendRows();
		gridVentas->SetCellValue(i,0, to_string(venta.GetID()));
		gridVentas->SetCellValue(i,1, to_string(venta.GetIDCliente()));
		gridVentas->SetCellValue(i,2, to_string(venta.GetTotal()));
		gridVentas->SetCellValue(i,3, venta.GetStrFecha());
	}
	gridVentas->SetColFormatFloat(1,-1,0);
	gridVentas->SetColFormatFloat(2,-1,2);
}


/// -- Actualizar GRID DETALLES
void VentasFrame::ActualizarGridDetalles(int id_venta){
	if(gridDetalles->GetNumberRows() != 0){
		gridDetalles->DeleteRows(0,gridDetalles->GetNumberRows());
	}
	
	if(id_venta != 0){
		vector<VentaDetalle> detalles = sistema->GetDetallesByIDVenta(id_venta);
		for(int i=0; i<detalles.size(); i++){
			gridDetalles->AppendRows();
			gridDetalles->SetCellValue(i,0, detalles[i].GetDescripcion());
			gridDetalles->SetCellValue(i,1, to_string(detalles[i].GetValorVendido()));
			gridDetalles->SetCellValue(i,2, to_string(detalles[i].GetCantidad()));
			gridDetalles->SetCellValue(i,3, to_string(detalles[i].GetSubtotal()));
		}
		gridDetalles->SetColFormatFloat(1,-1,2);
		gridDetalles->SetColFormatFloat(2,-1,0);
		gridDetalles->SetColFormatFloat(3,-1,2);
	}
}

// Ejecutada en btn "Actualizar"
void VentasFrame::ActualizarGrids( wxCommandEvent& event )  {
	sistema->LoadVentas();
	sistema->LoadDetallesVenta();
	ActualizarGridVentas();
	ActualizarGridDetalles(0);
}


/// -- AGREGAR
void VentasFrame::DisplayAddVenta( wxCommandEvent& event )  {
	AddVenta *win = new AddVenta(this,sistema);
	if(win->ShowModal() == 1){
		ActualizarGridVentas();
		ActualizarGridDetalles(0);
	}
}


/// -- MOSTRAR DETALLE
void VentasFrame::DisplayDetalleVenta( wxCommandEvent& event )  {
	if(gridVentas->GetNumberRows() != 0){
		int row = gridVentas->GetGridCursorRow();
		int id_venta = stoi(wx_to_std(gridVentas->GetCellValue(row,0)));
		ActualizarGridDetalles(id_venta);
	}
}


/// -- ELIMINAR
void VentasFrame::EliminarVenta( wxCommandEvent& event )  {
	if(gridVentas->GetNumberRows() == 0){
		wxMessageBox("No hay ventas","Error",wxOK|wxICON_ERROR);
	} else {
		int choice = wxMessageBox("�Esta seguro?","Advertencia",wxYES_NO|wxICON_QUESTION);
		if(choice == wxYES){
			int id = stoi(wx_to_std(gridVentas->GetCellValue(gridVentas->GetGridCursorRow(),0)));
			sistema->DeleteVenta(id);
			
			ActualizarGridVentas();
			ActualizarGridDetalles(0);
		}
	}
}


/// -- ORDENAR
void VentasFrame::OrdenarGrid( wxGridEvent& event )  {
	int col = event.GetCol();
	switch (col){
	case 0: sistema->OrdenarVentas(ID_VENTA); break;
	case 1: sistema->OrdenarVentas(IDCLIENTE); break;
	case 2: sistema->OrdenarVentas(TOTAL); break;
	case 3: sistema->OrdenarVentas(FECHA); break;
	}
	ActualizarGridVentas();
	ActualizarGridDetalles(0);
}


