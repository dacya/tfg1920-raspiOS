#include <ui/views/status_bar.h>
#include <ui/layouts.h>

VIEW_GROUP statusBarView;
VIEW brand;
VIEW time;
VIEW version;

void init_status_bar(int width, int height) {
    new_view_group(&statusBarView, width, height, 0, 0, horizontal_fitted_linear_layout);
    statusBarView.view.bgColor = BG_COLOR;

    new_view(&brand, 0, 0, 0, 0);
    brand.text = "artOS";
    brand.bgColor = BG_COLOR;
    addView(&statusBarView, &brand);

    new_view(&time, 0, 0, 0, 0);
    time.text = "12:00";
    time.textAlign = CENTER;
    time.bgColor = BLACK;
    addView(&statusBarView, &time);

    new_view(&version, 0, 0, 0, 0);
    addView(&statusBarView, &version);
    version.text = "1.0";
    version.textAlign = RIGHT;
    version.bgColor = BG_COLOR;
}