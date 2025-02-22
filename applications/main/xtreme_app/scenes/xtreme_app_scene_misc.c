#include "../xtreme_app.h"

enum VarItemListIndex {
    VarItemListIndexSortDirsFirst,
    VarItemListIndexChangeDeviceName,
    VarItemListIndexExperimentalOptions,
    VarItemListIndexDarkMode,
    VarItemListIndexLeftHanded,
};

void xtreme_app_scene_misc_var_item_list_callback(void* context, uint32_t index) {
    XtremeApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

static void xtreme_app_scene_misc_sort_dirs_first_changed(VariableItem* item) {
    XtremeApp* app = variable_item_get_context(item);
    bool value = variable_item_get_current_value_index(item);
    variable_item_set_current_value_text(item, value ? "ON" : "OFF");
    XTREME_SETTINGS()->sort_dirs_first = value;
    app->save_settings = true;
}

static void xtreme_app_scene_misc_dark_mode_changed(VariableItem* item) {
    XtremeApp* app = variable_item_get_context(item);
    bool value = variable_item_get_current_value_index(item);
    variable_item_set_current_value_text(item, value ? "ON" : "OFF");
    XTREME_SETTINGS()->dark_mode = value;
    app->save_settings = true;
}

static void xtreme_app_scene_misc_left_handed_changed(VariableItem* item) {
    XtremeApp* app = variable_item_get_context(item);
    bool value = variable_item_get_current_value_index(item);
    variable_item_set_current_value_text(item, value ? "ON" : "OFF");
    XTREME_SETTINGS()->left_handed = value;
    app->save_settings = true;
}

void xtreme_app_scene_misc_on_enter(void* context) {
    XtremeApp* app = context;
    XtremeSettings* xtreme_settings = XTREME_SETTINGS();
    VariableItemList* var_item_list = app->var_item_list;
    VariableItem* item;

    item = variable_item_list_add(
        var_item_list, "Sort Dirs First", 2, xtreme_app_scene_misc_sort_dirs_first_changed, app);
    variable_item_set_current_value_index(item, xtreme_settings->sort_dirs_first);
    variable_item_set_current_value_text(item, xtreme_settings->sort_dirs_first ? "ON" : "OFF");

    variable_item_list_add(var_item_list, "Change Device Name", 0, NULL, app);

    variable_item_list_add(var_item_list, "      Experimental Options:", 0, NULL, app);

    item = variable_item_list_add(
        var_item_list, "Dark Mode", 2, xtreme_app_scene_misc_dark_mode_changed, app);
    variable_item_set_current_value_index(item, xtreme_settings->dark_mode);
    variable_item_set_current_value_text(item, xtreme_settings->dark_mode ? "ON" : "OFF");

    item = variable_item_list_add(
        var_item_list, "Left Handed", 2, xtreme_app_scene_misc_left_handed_changed, app);
    variable_item_set_current_value_index(item, xtreme_settings->left_handed);
    variable_item_set_current_value_text(item, xtreme_settings->left_handed ? "ON" : "OFF");

    variable_item_list_set_enter_callback(
        var_item_list, xtreme_app_scene_misc_var_item_list_callback, app);

    variable_item_list_set_selected_item(
        var_item_list, scene_manager_get_scene_state(app->scene_manager, XtremeAppSceneMisc));

    view_dispatcher_switch_to_view(app->view_dispatcher, XtremeAppViewVarItemList);
}

bool xtreme_app_scene_misc_on_event(void* context, SceneManagerEvent event) {
    XtremeApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, XtremeAppSceneMisc, event.event);
        consumed = true;
        switch(event.event) {
        case VarItemListIndexChangeDeviceName:
            scene_manager_next_scene(app->scene_manager, XtremeAppSceneMiscRename);
            break;
        default:
            break;
        }
    }

    return consumed;
}

void xtreme_app_scene_misc_on_exit(void* context) {
    XtremeApp* app = context;
    variable_item_list_reset(app->var_item_list);
}
