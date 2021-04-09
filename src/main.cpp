
#include <gempyre.h>
#include <gempyre_utils.h>
#include <gempyre_graphics.h>

#include "mandelbrot_resource.h"

#include "mandelbrotdraw.h"

int main(int /*argc*/, char** /*argv*/) {
    Gempyre::setDebug();

    Gempyre::Ui ui({{"/mandelbrot.html", Mandelbrothtml}, {"/mandelbrot.css", Mandelbrotcss}, {"/mandelbrot.png", Mandelbrotpng}},
                 "mandelbrot.html");
    Gempyre::CanvasElement canvas(ui, "canvas");
    Gempyre::Element iterations(ui, "iterations_slider");
    Gempyre::Element colors(ui, "color_slider");
    Gempyre::Element radius(ui, "radius");
    Gempyre::Element zooms(ui, "zooms");
    Gempyre::Graphics graphics(canvas);
    Gempyre::Graphics backupGraphics(canvas);
    Gempyre::Graphics blend(canvas);
    Gempyre::Element busy(ui, "busy");

    bool mousedown = false;
    int mousex;
    int mousey;

    std::unique_ptr<MandelbrotDraw> mandelbrot;
    std::vector<std::array<Mandelbrot::Number, 4>> coordinateStack;

    const auto updater = [&graphics, &busy](int c, int a) {
        if(c == 0) {
            busy.setAttribute("style", "display:inline");
        }
        if(c < a) {
            busy.setHTML("Calculating..." + std::to_string(c * 100 / a) + "%");
        }
        if(c == a) {
            busy.setAttribute("style", "display:none");
            graphics.update();
        }
    };

    auto rect = *canvas.rect();

    ui.setLogging(true);


    ui.onOpen([&]() {
        rect = *canvas.rect();
        const auto type = *canvas.type();
        gempyre_graphics_assert(type == "canvas", "the element is expected to be a canvas");

        ui.root().subscribe("scroll", [&rect, &canvas] (const auto) {
            rect = *canvas.rect();
        });

        iterations.subscribe("change",[&mandelbrot, &updater](const Gempyre::Event& ev){

            const auto value = *GempyreUtils::toOr<int>(ev.properties.at("value"));
            mandelbrot->setIterations(value);
            mandelbrot->update(updater);
        }, {"value"});

        colors.subscribe("change",[&mandelbrot, &updater](const Gempyre::Event& ev){
            const auto value = *GempyreUtils::toOr<int>(ev.properties.at("value"));
            mandelbrot->setColors(value);
            mandelbrot->update(updater);
        }, {"value"});

        canvas.subscribe("mousedown", [&mousex, &mousey, &mousedown, &rect, &graphics, &backupGraphics] (const Gempyre::Event& ev) {
            mousex = *GempyreUtils::toOr<int>(ev.properties.at("clientX")) - rect.x;
            mousey = *GempyreUtils::toOr<int>(ev.properties.at("clientY")) - rect.y;
            mousedown = true;
            backupGraphics = graphics.clone();
        }, {"clientX", "clientY"});

        canvas.subscribe("mouseup", [&mousex, &mousey, &mousedown, &rect, &graphics, &backupGraphics, &mandelbrot, &coordinateStack, &radius, &zooms, &updater](const Gempyre::Event& ev) {
            const auto mx = *GempyreUtils::toOr<int>(ev.properties.at("clientX")) - rect.x;
            const auto my = *GempyreUtils::toOr<int>(ev.properties.at("clientY")) - rect.y;
            mousedown = false;
            graphics = std::move(backupGraphics);
            const auto delta = std::max(mx - mousex, my - mousey);
            if(delta > 5) {
                mandelbrot->setRect(mousex, mousey, delta, delta);
                coordinateStack.push_back(mandelbrot->coords());
                mandelbrot->update(updater);
            }
            graphics.update();
            radius.setHTML(Mandelbrot::toString(mandelbrot->radius()));
            zooms.setHTML(std::to_string(coordinateStack.size() - 1));
        }, {"clientX", "clientY"});

        canvas.subscribe("mousemove", [&mousex, &mousey, &mousedown, &rect, &graphics, &backupGraphics, &blend](const Gempyre::Event& ev) {
            if(mousedown) {
                const auto mx = *GempyreUtils::toOr<int>(ev.properties.at("clientX")) - rect.x;
                const auto my = *GempyreUtils::toOr<int>(ev.properties.at("clientY")) - rect.y;
                blend.drawRect(Gempyre::Element::Rect{0, 0, rect.width, rect.height}, Gempyre::Graphics::pix(0x73, 0x73, 0x73, 0x83));
                blend.drawRect(Gempyre::Element::Rect{mousex, mousey, mx - mousex, my - mousey}, Gempyre::Graphics::pix(0,0,0,0));
                graphics.merge(backupGraphics);
                graphics.merge(blend);
                graphics.update();
            }
        }, {"clientX", "clientY"}, 200ms);

        canvas.subscribe("dblclick", [&coordinateStack, &mandelbrot, &radius, &zooms, &updater](const Gempyre::Event&){
            if(coordinateStack.size() == 1) {
              return;
            }
            coordinateStack.pop_back();
            mandelbrot->set(
                        coordinateStack.back()[0],
                        coordinateStack.back()[1],
                        coordinateStack.back()[2],
                        coordinateStack.back()[3]);
            mandelbrot->update(updater);
            radius.setHTML(Mandelbrot::toString(mandelbrot->radius()));
            zooms.setHTML(std::to_string(coordinateStack.size() - 1));
        });


        const auto ival = *GempyreUtils::toOr<int>(std::any_cast<std::string>(iterations.values()->at("value")));
        const auto cval = *GempyreUtils::toOr<int>(std::any_cast<std::string>(colors.values()->at("value")));


        graphics.create(rect.width, rect.height);
        coordinateStack.push_back({-2, -2, 2, 2});
        mandelbrot = std::make_unique<MandelbrotDraw>(graphics,
                coordinateStack.back()[0],
                coordinateStack.back()[1],
                coordinateStack.back()[2],
                coordinateStack.back()[3],
                ival);

        mandelbrot->setColors(cval);
        mandelbrot->update(updater);
        blend.create(rect.width, rect.height);

        radius.setHTML(Mandelbrot::toString(mandelbrot->radius()));
        zooms.setHTML(std::to_string(coordinateStack.size() - 1));

       // ui.startTimer(3000ms, true,[&canvas, &bytes](){
       //     canvas.paint(bytes);
       // });
    });

    ui.run();
    return 0;
}
