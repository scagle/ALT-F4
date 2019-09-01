#include <iostream>
#include <stdio.h>
#include <gst/gst.h>
#include <glib.h>

using namespace std;

static gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer data)
{
    GMainLoop *loop = (GMainLoop *) data;
    switch (GST_MESSAGE_TYPE(msg)) 
    {
        case GST_MESSAGE_EOS:
            g_print("End of stream\n");
            g_main_loop_quit(loop);
            break;

        case GST_MESSAGE_ERROR: {
            gchar *debug;
            GError *error;

            gst_message_parse_error(msg, &error, &debug);
            g_free(debug);

            g_printerr("Error: %s\n", error->message);
            g_error_free(error);

            g_main_loop_quit(loop);
            break;
        }
        default:
            break;
    }
    return TRUE;
}

int main(int argc, char *argv[])
{
    GMainLoop *loop;

    GstElement *pipeline, *videotestsrcm, *autovideosinkm;
    GstBus *bus;
    guint bus_watch_id;
    
    gst_init(&argc, &argv);
    loop = g_main_loop_new(NULL, FALSE);

    pipeline = gst_pipeline_new("camera-pipeline");
    videotestsrcm = gst_element_factory_make("camera-src", "cameratest");
    autovideosinkm = gst_element_factory_make("autovideosink", "videosink");

    if (!pipeline) 
    {
        g_printerr("pipeline element could not be created. Exiting.\n");
        return -1;
    }
    if (!videotestsrcm) 
    {
        g_printerr("videotestsrcm element could not be created. Exiting.\n");
        return -1;
    }
    if (!autovideosinkm) 
    {
        g_printerr("autovideosinkm element could not be created. Exiting.\n");
        return -1;
    }

    // message handler
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
    gst_object_unref(bus);

    // add elements into pipeline
    gst_bin_add_many(GST_BIN(pipeline), videotestsrcm, autovideosinkm, NULL);

    // link elements together
    gst_element_link(videotestsrcm, autovideosinkm);

    // "play" pipeline
    g_print("Now set pipeline in state playing");
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // iterate
    g_print("Running...\n");
    g_main_loop_run(loop);
    
    return 0;
}
