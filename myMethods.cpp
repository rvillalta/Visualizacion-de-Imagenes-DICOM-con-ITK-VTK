#include "myMethods.h"

// Implementación del método New() que simplemente crea una nueva instancia
SlicesSliderCallback* SlicesSliderCallback::New() {
    return new SlicesSliderCallback;
}

// Este método se llama cada vez que el usuario interactúa con el slider
void SlicesSliderCallback::Execute(vtkObject* caller, unsigned long eventId, void* callData) {
    // Convertimos el objeto genérico caller al tipo vtkSliderWidget
    auto slider = static_cast<vtkSliderWidget*>(caller);

    // Obtenemos la representación visual del slider (posición, valor, etc.)
    auto sliderRepresentation = static_cast<vtkSliderRepresentation2D*>(slider->GetRepresentation());

    // Extraemos el valor actual del slider y lo convertimos a entero (índice de slice)
    int slice = static_cast<int>(sliderRepresentation->GetValue());

    // Actualizamos el slice mostrado en el viewer
    viewer->SetSlice(slice);

    // Renderizamos la nueva imagen
    viewer->Render();
}
