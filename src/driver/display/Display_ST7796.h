#pragma once

#include <interface/DisplayInterface.h>
#include <lvgl.h>
#include <LovyanGFX.hpp>

// class Display_ST7796 : public DisplayInterface
class Display_ST7796
{
public:
    Display_ST7796(const uint32_t screenWidht = 320, const uint32_t screenHeight = 480);
    bool begin();
    bool setImage(const uint8_t image[]);
    bool setGifImage(const uint8_t gifImage[]);
    bool clear();

    void fillScreenBlack();
    void fillScreenRed();
    void fillScreenGreen();
    void fillScreenBlue();

private:
    LGFX_Device lgfx_d;

    static void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area,
                       lv_color_t *color_p);
    static void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

    void init_lcd();
    void init_touch();

    const uint32_t _screenWidth;
    const uint32_t _screenHeight;

    static constexpr lgfx::v1::Bus_SPI::config_t cfg_bus{
        .freq_write = 80000000,
        .freq_read = 16000000,
        .pin_sclk = 14,
        .pin_miso = 12,
        .pin_mosi = 13,
        .pin_dc = 2,
        .spi_mode = 0,
        .spi_3wire = false,
        .use_lock = true,
        // .dma_channel = 1,
        .dma_channel = SPI_DMA_CH_AUTO,
        .spi_host = SPI2_HOST};

    static constexpr lgfx::v1::Panel_Device::config_t cfg_panel{
        .pin_cs = 15,
        .pin_rst = -1,
        .pin_busy = -1,
        .memory_width = 320,
        .memory_height = 480,
        .panel_width = 320,
        .panel_height = 480,
        .offset_x = 0,
        .offset_y = 0,
        .offset_rotation = 0,
        .dummy_read_pixel = 8,
        .dummy_read_bits = 1,
        .readable = false,
        .invert = false,
        .rgb_order = false,
        .dlen_16bit = false,
        .bus_shared = false};

    static constexpr lgfx::v1::Light_PWM::config_t cfg_light{
        .freq = 44100,
        .pin_bl = 27,
        .pwm_channel = 7,
        .invert = false};

    static constexpr lgfx::v1::ITouch::config_t cfg_touch{
        .freq = 1000000,
        .x_min = 222,
        .x_max = 3367,
        .y_min = 192,
        .y_max = 3732,
        .bus_shared = true,
        .pin_int = -1,
        .offset_rotation = 6,

        // .pin_sclk = 14,
        // .pin_mosi = 13,
        // .pin_miso = 12,
        // .pin_cs = 33
    };
};
