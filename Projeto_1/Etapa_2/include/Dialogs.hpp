#ifndef DIALOGS_HPP
#define DIALOGS_HPP

#include <gtk/gtk.h>
#include <string>
#include "Objects.hpp"

#define UI_FILE "window.glade"

enum class rotateType { OBJECT, WORLD, POINT };

class Dialog
{
    public:
        Dialog(){}
        ~Dialog(){ destroy(); }
        int run();
        virtual void destroy();
    protected:
        GtkWidget *_dialog;
    private:
};

class PointDialog : public Dialog
{
    public:
        PointDialog(GtkBuilder* builder);
        std::string const getName(){ return gtk_entry_get_text(GTK_ENTRY(_entryName)); }
        double getX(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_entryX)); }
        double getY(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_entryY)); }
    protected:
    private:
        GtkWidget *_entryX, *_entryY, *_entryName;
};

class LineDialog : public Dialog
{
    public:
        LineDialog(GtkBuilder* builder);
        std::string const getName(){ return gtk_entry_get_text(GTK_ENTRY(_entryName)); }
        double getX1(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_entryX1)); }
        double getY1(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_entryY1)); }
        double getX2(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_entryX2)); }
        double getY2(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_entryY2)); }
    protected:
    private:
        GtkWidget *_entryX1, *_entryY1,
            *_entryX2, *_entryY2, *_entryName;
};

class PolygonDialog : public Dialog
{
    public:
        PolygonDialog(GtkBuilder* builder);
        ~PolygonDialog(){ destroy(); }
        void destroy();

        std::string const getName(){ return gtk_entry_get_text(GTK_ENTRY(_entryName)); }
        void getCoords(Coordinates& coords);
        bool shouldBeFilled(){ return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(_checkFilled)); }

        // Events
        void onClickEvent();//Adiciona coordenada
        void onEditCelEvent(GtkCellRendererText *cell,
                            const gchar *path_string,
                            const gchar *new_text, int column);//Edita coordenada ja adicionada
    protected:
    private:
        GtkTreeModel *_model;
        GtkWidget *_entryX, *_entryY, *_checkFilled, *_entryName;
};

class TranslateDialog : public Dialog
{
    public:
        TranslateDialog(GtkBuilder* builder);
        double getDX(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_entryDX)); }
        double getDY(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_entryDY)); }

    private:
        GtkWidget *_entryDX, *_entryDY;
};

class ScaleDialog : public Dialog
{
    public:
        ScaleDialog(GtkBuilder* builder);
        double getSX(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_entrySX)); }
        double getSY(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_entrySY)); }

    private:
        GtkWidget *_entrySX, *_entrySY;
};

class RotateDialog : public Dialog
{
    public:
        RotateDialog(GtkBuilder* builder);

        double getAngulo(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_entryAngulo)); }
        double getCX(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_entryCX)); }
        double getCY(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_entryCY)); }
        rotateType getRotateType(); // Retorna o tipo de rotacao escolhido pelo usuario

    private:
        GtkWidget *_entryAngulo, *_entryCX, *_entryCY,
            *_objCenter, *_worldCenter, *_pntCenter;

        bool isActive(GtkWidget *widget){ return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)); }
};

#endif // DIALOGS_HPP
