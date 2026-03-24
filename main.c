#include "lvgl/lvgl.h"
#include <stdio.h>
#include <stdlib.h>

/* Prototipler */
static lv_display_t * hal_init(int32_t w, int32_t h);
static void create_main_menu(void);
static void havya_event_cb(lv_event_t * e);
static void havya_ek_event_cb(lv_event_t * e);
static void sogutucu_event_cb(lv_event_t * e);
static void go_back_event_cb(lv_event_t * e);
static void havya_screen_create(void);
static void havya_ek_screen_create(void);
static void sogutucu_screen_create(void);
static void arc_event_cb(lv_event_t * e);
static void digit_timer_cb(lv_timer_t * timer);
static void reboot_event_cb(lv_event_t * e);
static void temp_area_keyboard_cb(lv_event_t * e);

/* Globaller */
static lv_obj_t * main_screen;
static lv_obj_t * havya_screen;
static lv_obj_t * havya_ek_screen;
static lv_obj_t * sogutucu_screen;

/* Havya ekran değişkenleri */
static int target_val = 0;
static int current_val = 0;
static lv_obj_t *arc_label;
static lv_obj_t *digit_label;
static lv_obj_t *unit_label;
static lv_timer_t *digit_timer = NULL;

/* Ek özellikler */
static lv_obj_t *temp_area = NULL;
static lv_obj_t *wait_temp_area = NULL;
static lv_obj_t *wait_time_area = NULL;
static lv_obj_t *sleep_mode_time_area = NULL;
static int start_temp = 250;      /* Başlangıç sıcaklığı */
static int wait_temp = 180;       /* Bekleme sıcaklığı */
static int wait_time = 30;       /* Bekleme süresi */
static int sleep_mode_time = 60; /* Uyku modu süresi */

/* main */
int main(int argc, char **argv)
{
    lv_init();
    hal_init(320, 240);
    create_main_menu();

    while(1) {
        lv_timer_handler();
        lv_tick_inc(5);
    }

    lv_deinit();
    return 0;
}

/* HAL - SDL simülasyonu */
static lv_display_t * hal_init(int32_t w, int32_t h)
{
    lv_group_set_default(lv_group_create());
    lv_display_t * disp = lv_sdl_window_create(w, h);

    lv_indev_t * mouse = lv_sdl_mouse_create();
    lv_indev_set_group(mouse, lv_group_get_default());
    lv_indev_set_display(mouse, disp);

    lv_display_set_default(disp);
    return disp;
}

/* Ana Menü */
static void create_main_menu(void)
{
    main_screen = lv_scr_act();

    static lv_style_t bg_style;
    lv_style_init(&bg_style);
    lv_style_set_bg_color(&bg_style, lv_color_make(230, 200, 255));
    lv_style_set_bg_grad_color(&bg_style, lv_color_make(150, 100, 255));
    lv_style_set_bg_grad_dir(&bg_style, LV_GRAD_DIR_VER);
    lv_style_set_bg_opa(&bg_style, LV_OPA_COVER);
    lv_obj_add_style(main_screen, &bg_style, 0);

    lv_obj_t *title = lv_label_create(main_screen);
    lv_label_set_text(title, "2in1 Havya Istasyonu");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t *btn_havya = lv_btn_create(main_screen);
    lv_obj_set_size(btn_havya, 150, 60);
    lv_obj_align(btn_havya, LV_ALIGN_CENTER, 0, -40);
    lv_obj_set_style_bg_color(btn_havya, lv_color_make(187, 106, 238), 0);
    lv_obj_t *label1 = lv_label_create(btn_havya);
    lv_label_set_text(label1, "Havya");
    lv_obj_center(label1);
    lv_obj_add_event_cb(btn_havya, havya_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_sogutucu = lv_btn_create(main_screen);
    lv_obj_set_size(btn_sogutucu, 150, 60);
    lv_obj_align(btn_sogutucu, LV_ALIGN_CENTER, 0, 40);
    lv_obj_set_style_bg_color(btn_sogutucu, lv_color_make(187, 106, 238), 0);
    lv_obj_t *label2 = lv_label_create(btn_sogutucu);
    lv_label_set_text(label2, "Sogutucu");
    lv_obj_center(label2);
    lv_obj_add_event_cb(btn_sogutucu, sogutucu_event_cb, LV_EVENT_CLICKED, NULL);
}

/* Havya ekranı */
static void havya_screen_create(void)
{
    if(havya_screen) return;
    havya_screen = lv_obj_create(NULL);

    static lv_style_t bg_style;
    lv_style_init(&bg_style);
    lv_style_set_bg_color(&bg_style, lv_color_make(173,216,230));
    lv_style_set_bg_grad_color(&bg_style, lv_color_make(255,182,193));
    lv_style_set_bg_grad_dir(&bg_style, LV_GRAD_DIR_VER);
    lv_style_set_bg_opa(&bg_style, LV_OPA_COVER);
    lv_obj_add_style(havya_screen, &bg_style, 0);

    lv_obj_t *label = lv_label_create(havya_screen);
    lv_label_set_text(label, "HAVYA SICAKLIK AYARI !");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);

    /* Arc */
    lv_obj_t *arc = lv_arc_create(havya_screen);
    lv_obj_set_size(arc, 100, 100);
    lv_obj_align(arc, LV_ALIGN_LEFT_MID, 10, -25);
    lv_arc_set_range(arc, 0, 420);
    lv_arc_set_value(arc, 0);
    lv_arc_set_bg_angles(arc, 0, 330);

    arc_label = lv_label_create(havya_screen); 
    lv_label_set_text(arc_label, "0");
    lv_obj_align_to(arc_label, arc, LV_ALIGN_CENTER, 0, 0);

    /* Digit box */
    digit_label = lv_label_create(havya_screen);
    current_val = start_temp;
    char buf[8];
    snprintf(buf, sizeof(buf), "%03d", current_val);
    lv_label_set_text(digit_label, buf);
    lv_obj_set_style_bg_color(digit_label, lv_color_white(), 0);
    lv_obj_set_style_border_color(digit_label, lv_color_black(), 0);
    lv_obj_set_style_border_width(digit_label, 3, 0);
    lv_obj_set_style_text_font(digit_label, &lv_font_montserrat_40, 0);
    lv_obj_set_width(digit_label, 80);
    lv_obj_align(digit_label, LV_ALIGN_LEFT_MID, 150, -25);
    lv_label_set_long_mode(digit_label, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_align(digit_label, LV_TEXT_ALIGN_CENTER, 0);

    unit_label = lv_label_create(havya_screen);
    lv_label_set_text(unit_label, "°C");
    lv_obj_set_style_text_font(unit_label, &lv_font_montserrat_40, 0);
    lv_obj_align_to(unit_label, digit_label, LV_ALIGN_OUT_RIGHT_MID, 15, 0);

    lv_obj_add_event_cb(arc, arc_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* Geri butonu */
    lv_obj_t *btn_back = lv_btn_create(havya_screen);
    lv_obj_set_size(btn_back, 25, 25);
    lv_obj_align(btn_back, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_set_style_radius(btn_back, LV_RADIUS_CIRCLE, 0);
    lv_obj_t *lbl = lv_label_create(btn_back);
    lv_label_set_text(lbl, "<");
    lv_obj_center(lbl);
    lv_obj_add_event_cb(btn_back, go_back_event_cb, LV_EVENT_CLICKED, main_screen);

    /* Ek Özellikler butonu */
    lv_obj_t *btn_ek = lv_btn_create(havya_screen);
    lv_obj_set_size(btn_ek, 150, 50);
    lv_obj_align(btn_ek, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_style_bg_color(btn_ek, lv_color_make(187, 106, 238), 0);
    lv_obj_t *lbl_ek = lv_label_create(btn_ek);
    lv_label_set_text(lbl_ek, "Ek Ozellikler");
    lv_obj_center(lbl_ek);
    lv_obj_add_event_cb(btn_ek, havya_ek_event_cb, LV_EVENT_CLICKED, NULL);
}

/* Ek Özellikler ekranı */
static void havya_ek_screen_create(void)
{
    if(havya_ek_screen) return;
    havya_ek_screen = lv_obj_create(NULL);

    static lv_style_t bg_style;
    lv_style_init(&bg_style);
    lv_style_set_bg_color(&bg_style, lv_color_make(181, 225, 215));
    lv_style_set_bg_grad_color(&bg_style, lv_color_make(124, 61, 76));
    lv_style_set_bg_grad_dir(&bg_style, LV_GRAD_DIR_VER);
    lv_style_set_bg_opa(&bg_style, LV_OPA_COVER);
    lv_obj_add_style(havya_ek_screen, &bg_style, 0);

    lv_obj_t *label = lv_label_create(havya_ek_screen);
    lv_label_set_text(label, "Ek Ozellikler");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t *btn_back = lv_btn_create(havya_ek_screen);
    lv_obj_set_size(btn_back, 25, 25);
    lv_obj_align(btn_back, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_set_style_radius(btn_back, LV_RADIUS_CIRCLE, 0);
    lv_obj_t *lbl = lv_label_create(btn_back);
    lv_label_set_text(lbl, "<");
    lv_obj_center(lbl);
    lv_obj_add_event_cb(btn_back, go_back_event_cb, LV_EVENT_CLICKED, havya_screen);

    /* Başlangıç sıcaklığı */
    lv_obj_t *temp_cont = lv_obj_create(havya_ek_screen);
    lv_obj_set_size(temp_cont, 250, 40);
    lv_obj_align(temp_cont, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_set_style_bg_color(temp_cont, lv_color_white(), 0);
    lv_obj_set_style_border_color(temp_cont, lv_color_black(), 0);
    lv_obj_set_style_border_width(temp_cont, 2, 0);

    lv_obj_t *temp_label = lv_label_create(temp_cont);
    lv_label_set_text(temp_label, "Baslangic Sicakligi (°C)");
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_12, 0);
    lv_obj_align(temp_label, LV_ALIGN_LEFT_MID, 5, 0);

    temp_area = lv_textarea_create(temp_cont);
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", start_temp);
    lv_textarea_set_text(temp_area, buf);
    lv_obj_set_width(temp_area, 60);
    lv_obj_align(temp_area, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_textarea_set_max_length(temp_area, 3); // Maksimum 3 karakter
    lv_textarea_set_accepted_chars(temp_area, "0123456789"); // Sadece rakamlar
    lv_textarea_set_one_line(temp_area, true); // Tek satır

    /* Bekleme sıcaklığı */
    lv_obj_t *wait_cont = lv_obj_create(havya_ek_screen);
    lv_obj_set_size(wait_cont, 250, 40);
    lv_obj_align(wait_cont, LV_ALIGN_TOP_MID, 0, 80);
    lv_obj_set_style_bg_color(wait_cont, lv_color_white(), 0);
    lv_obj_set_style_border_color(wait_cont, lv_color_black(), 0);
    lv_obj_set_style_border_width(wait_cont, 2, 0);

    lv_obj_t *wait_label = lv_label_create(wait_cont);
    lv_label_set_text(wait_label, "Bekleme Sicakligi (°C)");
    lv_obj_set_style_text_font(wait_label, &lv_font_montserrat_12, 0);
    lv_obj_align(wait_label, LV_ALIGN_LEFT_MID, 5, 0);

    wait_temp_area = lv_textarea_create(wait_cont);
    snprintf(buf, sizeof(buf), "%d", wait_temp);
    lv_textarea_set_text(wait_temp_area, buf);
    lv_obj_set_width(wait_temp_area, 60);
    lv_obj_align(wait_temp_area, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_textarea_set_max_length(wait_temp_area, 3);
    lv_textarea_set_accepted_chars(wait_temp_area, "0123456789");
    lv_textarea_set_one_line(wait_temp_area, true);

    /* Bekleme süresi */
    lv_obj_t *wait_time_cont = lv_obj_create(havya_ek_screen);
    lv_obj_set_size(wait_time_cont, 250, 40);
    lv_obj_align(wait_time_cont, LV_ALIGN_TOP_MID, 0, 120);
    lv_obj_set_style_bg_color(wait_time_cont, lv_color_white(), 0);
    lv_obj_set_style_border_color(wait_time_cont, lv_color_black(), 0);
    lv_obj_set_style_border_width(wait_time_cont, 2, 0);

    lv_obj_t *wait_time_label = lv_label_create(wait_time_cont);
    lv_label_set_text(wait_time_label, "Bekleme Suresi (dk)");
    lv_obj_set_style_text_font(wait_time_label, &lv_font_montserrat_12, 0);
    lv_obj_align(wait_time_label, LV_ALIGN_LEFT_MID, 5, 0);

    wait_time_area = lv_textarea_create(wait_time_cont);
    snprintf(buf, sizeof(buf), "%d", wait_time);
    lv_textarea_set_text(wait_time_area, buf);
    lv_obj_set_width(wait_time_area, 60);
    lv_obj_align(wait_time_area, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_textarea_set_max_length(wait_time_area, 3);
    lv_textarea_set_accepted_chars(wait_time_area, "0123456789");
    lv_textarea_set_one_line(wait_time_area, true);

    /* Uyku modu süresi */
    lv_obj_t *sleep_mode_time_cont = lv_obj_create(havya_ek_screen);
    lv_obj_set_size(sleep_mode_time_cont, 250, 40);
    lv_obj_align(sleep_mode_time_cont, LV_ALIGN_TOP_MID, 0, 160);
    lv_obj_set_style_bg_color(sleep_mode_time_cont, lv_color_white(), 0);
    lv_obj_set_style_border_color(sleep_mode_time_cont, lv_color_black(), 0);
    lv_obj_set_style_border_width(sleep_mode_time_cont, 2, 0);

    lv_obj_t *sleep_mode_time_label = lv_label_create(sleep_mode_time_cont);
    lv_label_set_text(sleep_mode_time_label, "Uyku Modu Suresi (dk)");
    lv_obj_set_style_text_font(sleep_mode_time_label, &lv_font_montserrat_12, 0);
    lv_obj_align(sleep_mode_time_label, LV_ALIGN_LEFT_MID, 5, 0);

    sleep_mode_time_area = lv_textarea_create(sleep_mode_time_cont);
    snprintf(buf, sizeof(buf), "%d", sleep_mode_time);
    lv_textarea_set_text(sleep_mode_time_area, buf);
    lv_obj_set_width(sleep_mode_time_area, 60);
    lv_obj_align(sleep_mode_time_area, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_textarea_set_max_length(sleep_mode_time_area, 3);
    lv_textarea_set_accepted_chars(sleep_mode_time_area, "0123456789");
    lv_textarea_set_one_line(sleep_mode_time_area, true);

    /*reboot butonu*/
    lv_obj_t *btn_reboot = lv_btn_create(havya_ek_screen);
    lv_obj_set_size(btn_reboot, 150, 30);
    lv_obj_align(btn_reboot, LV_ALIGN_BOTTOM_MID, 0, -3); // Ekranın alt ortasına hizala
    lv_obj_set_style_bg_color(btn_reboot, lv_color_make(255, 99, 71), 0); // Turuncu kırmızı renk
    lv_obj_t *lbl_reboot = lv_label_create(btn_reboot);
    lv_label_set_text(lbl_reboot, "Reboot");
    lv_obj_center(lbl_reboot);
    lv_obj_add_event_cb(btn_reboot, reboot_event_cb, LV_EVENT_CLICKED, NULL);

    lv_indev_t *kb = lv_sdl_keyboard_create();
    lv_indev_set_group(kb, lv_group_get_default());

    lv_obj_add_event_cb(temp_area, temp_area_keyboard_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(wait_temp_area, temp_area_keyboard_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(wait_time_area, temp_area_keyboard_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(sleep_mode_time_area, temp_area_keyboard_cb, LV_EVENT_ALL, NULL);

}

/* Reboot buton callback */
static void reboot_event_cb(lv_event_t * e)
{
    // Varsayılan değerler
    start_temp = 250;
    wait_temp = 180;
    wait_time = 30;
    sleep_mode_time = 60;

    char buf[8];

    // Textareaları güncelle
    snprintf(buf, sizeof(buf), "%d", start_temp);
    lv_textarea_set_text(temp_area, buf);

    snprintf(buf, sizeof(buf), "%d", wait_temp);
    lv_textarea_set_text(wait_temp_area, buf);

    snprintf(buf, sizeof(buf), "%d", wait_time);
    lv_textarea_set_text(wait_time_area, buf);

    snprintf(buf, sizeof(buf), "%d", sleep_mode_time);
    lv_textarea_set_text(sleep_mode_time_area, buf);

    // Eğer havya ekranı açıksa göstergeleri de güncelle
    current_val = start_temp;
    if(digit_label) {
        snprintf(buf, sizeof(buf), "%03d", current_val);
        lv_label_set_text(digit_label, buf);
        lv_obj_set_style_bg_color(digit_label, lv_color_white(), 0);
    }
}


/* Textarea callback */
static void temp_area_keyboard_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_READY || code == LV_EVENT_DEFOCUSED) {
        lv_obj_t *ta = lv_event_get_target(e); // Hangi textarea olduğunu al -- nereye metin giriliyor
        const char *txt = lv_textarea_get_text(ta); // Kullanıcının yazdığı metni al
        int val = atoi(txt); // Metni tamsayıya -int- çevir

        if(ta == temp_area) { //başlangıç sıcaklığı için işlem yapılıyorsa
            if(val < 0 || val > 420) val = start_temp;  //geçersizse eski değeri koru
            start_temp = val; // Başlangıç sıcaklığını güncelle
            if(target_val == 0 && current_val != start_temp) { //havya kapalıysa ekrandaki değeri de güncellemelisin
                current_val = start_temp; //gösterge değerini başlangıç sıcaklığına ayarla
                char big[8]; 
                snprintf(big, sizeof(big), "%03d", current_val);
                lv_label_set_text(digit_label, big);
            }
        }
        else if(ta == wait_temp_area) { //bekleme sıcaklığı için işlem yapılıyorsa
            if(val < 0 || val > 420) val = 180; //geçersizse eski değeri koru
            wait_temp = val; // Bekleme sıcaklığını güncelle
        }
        else if(ta == wait_time_area) { //bekleme süresi için işlem yapılıyorsa
            if(val < 0 || val > 100) val = 30; //geçersizse eski değeri koru
            wait_time = val; // Bekleme süresini güncelle
        }
        else if(ta == sleep_mode_time_area) { //uyku modu süresi için işlem yapılıyorsa
            if(val < 0 || val > 120) val = 60; //geçersizse eski değeri koru
            sleep_mode_time = val; // Uyku modu süresini güncelle
        }

        char buf[8]; //yeni atama yapıldı
        snprintf(buf, sizeof(buf), "%d", val); // Girilen değeri biçimlendir --doğru formata getir
        lv_textarea_set_text(ta, buf); // Textarea'yı güncelle

        if(ta == temp_area) {
            start_temp = val; // Başlangıç sıcaklığını güncelle
            if(target_val == 0 && current_val != start_temp) { //havya kapalıysa ekrandaki değeri güncellemelisin
                current_val = start_temp; //gösterge değerini başlangıç sıcaklığına ayarla
                char big[8];
                snprintf(big, sizeof(big), "%03d", current_val);
                lv_label_set_text(digit_label, big); // Göstergeyi güncelle
            }
        } else if(ta == wait_temp_area) { //bekleme sıcaklığı için işlem yapılıyorsa değeri değiştirip kaydetmek yeterli
            wait_temp = val; // Bekleme sıcaklığını güncelle
        }
        else if(ta == wait_time_area) { //bekleme süresi için işlem yapılıyorsa değeri değiştirip kaydetmek yeterli
            wait_time = val; // Bekleme süresini güncelle
        }
    }
}

/* Soğutucu ekranı */
static void sogutucu_screen_create(void)
{
    if(sogutucu_screen) return;
    sogutucu_screen = lv_obj_create(NULL);

    lv_obj_t *label = lv_label_create(sogutucu_screen);
    lv_label_set_text(label, "Sogutucu Sekmesi (bos)");
    lv_obj_center(label);

    lv_obj_t *btn_back = lv_btn_create(sogutucu_screen);
    lv_obj_set_size(btn_back, 25, 25);
    lv_obj_align(btn_back, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_set_style_radius(btn_back, LV_RADIUS_CIRCLE, 0);
    lv_obj_t *lbl = lv_label_create(btn_back);
    lv_label_set_text(lbl, "<");
    lv_obj_center(lbl);
    lv_obj_add_event_cb(btn_back, go_back_event_cb, LV_EVENT_CLICKED, main_screen);
}

/* Callback helpers */
static void havya_event_cb(lv_event_t * e) { //havya butonuna tıklandığında
    if(!havya_screen) havya_screen_create(); //havya ekranı oluşturulmamışsa oluştur
    lv_scr_load(havya_screen); //havya ekranını yükle
}
static void havya_ek_event_cb(lv_event_t * e) { //ek özellikler butonuna tıklandığında
    if(!havya_ek_screen) havya_ek_screen_create(); //ek özellikler ekranı oluşturulmamışsa oluştur
    lv_scr_load(havya_ek_screen); //ek özellikler ekranını yükle
}
static void sogutucu_event_cb(lv_event_t * e) { //soğutucu butonuna tıklandığında
    if(!sogutucu_screen) sogutucu_screen_create(); //soğutucu ekranı oluşturulmamışsa oluştur
    lv_scr_load(sogutucu_screen); //soğutucu ekranını yükle
}
static void go_back_event_cb(lv_event_t * e) {  //geri butonuna tıklandığında
    lv_obj_t *target_screen = (lv_obj_t *)lv_event_get_user_data(e); //hedef ekranı al
    lv_scr_load(target_screen); //hedef ekranı yükle
}

/* Arc event */
static void arc_event_cb(lv_event_t * e) //arc değeri değiştiğinde
{
    lv_obj_t * arc = lv_event_get_target(e); //arc objesini al
    target_val = lv_arc_get_value(arc); //arc'in değerini hedef değere ata

    char buf[8];
    snprintf(buf, sizeof(buf), "%d", target_val); //hedef değeri biçimlendir
    lv_label_set_text(arc_label, buf); //arc label'ı güncelle

    if(digit_timer) { //eğer digit timer zaten varsa
        lv_timer_del(digit_timer);  //timer değerini sil
        digit_timer = NULL; //timer'ı sıfırla
    }
    digit_timer = lv_timer_create(digit_timer_cb, 50, NULL); //yeni bir digit timer oluştur
}

/* Digit animasyon timer */
static void digit_timer_cb(lv_timer_t * timer) //digit timer callback fonksiyonu
{
    static int first_step = 1;
    int prev_val = current_val; //önceki değeri sakla

    if(current_val < target_val) current_val++; //hedef değere doğru sıcaklık değerini bir arttır
    else if(current_val > target_val) current_val--; //hedef değere doğru sıcaklık değerini bir azalt

    char buf[8];
    snprintf(buf, sizeof(buf), "%03d", current_val); //üç haneli biçimlendir
    lv_label_set_text(digit_label, buf); //digit label'ı güncelle

    if(current_val == target_val) {     //hedef değere ulaşıldıysa
        lv_obj_set_style_bg_color(digit_label, lv_color_hex(0x00FF00), 0); //yeşil arka plan
    } else if(first_step) { //ilk adımda
        if(current_val > start_temp) lv_obj_set_style_bg_color(digit_label, lv_color_hex(0xFF0000), 0); //kırmızı arka plan
        else if(current_val < start_temp) lv_obj_set_style_bg_color(digit_label, lv_color_hex(0x0000FF), 0); //mavi arka plan
    } else { //ilk adım değilse
        if(current_val > prev_val) lv_obj_set_style_bg_color(digit_label, lv_color_hex(0xFF0000), 0); //kırmızı arka plan
        else if(current_val < prev_val) lv_obj_set_style_bg_color(digit_label, lv_color_hex(0x0000FF), 0); //mavi arka plan
    }

    lv_obj_set_style_bg_opa(digit_label, LV_OPA_COVER, 0); 

    if(current_val == target_val) {   //hedef değere ulaşıldıysa
        lv_timer_del(digit_timer); //timer'ı sil
        digit_timer = NULL; //timer'ı sıfırla
        first_step = 1; //ilk adımı tekrar aktif et
    } else first_step = 0; //ilk adımı devre dışı bırak
}
