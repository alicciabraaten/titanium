/**
 * Appcelerator Kroll - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2008 Appcelerator, Inc. All Rights Reserved.
 */

#include "gst_media_binding.h"

namespace ti
{
	gboolean gst_bus_cb(GstBus *bus, GstMessage *message, gpointer data);

	GstSound::GstSound(std::string &path) : Sound(path),
		callback(NULL),
		pipeline(NULL)
	{
		this->looping = false;
		this->Load();
	}

	void GstSound::Load()
	{
		// Clean up old stream, if around
		if (pipeline != NULL)
		{
			gst_element_set_state(this->pipeline, GST_STATE_NULL);
			gst_object_unref(GST_OBJECT(this->pipeline));
		}

		this->state = STOPPED;
		this->pipeline = gst_element_factory_make("playbin", NULL);
		g_object_set(G_OBJECT(pipeline), "uri", url.c_str(), NULL);

		// Add a callback to listen for GST bus messages
		GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
		gst_bus_add_watch(bus, gst_bus_cb, this);
		gst_object_unref(bus);
	}

	GstSound::~GstSound()
	{
		gst_element_set_state(this->pipeline, GST_STATE_NULL);
		gst_object_unref(GST_OBJECT(this->pipeline));
	}

	void GstSound::Play()
	{
		if (this->state != PLAYING)
		{
			this->state = PLAYING;
			gst_element_set_state(this->pipeline, GST_STATE_PLAYING);
		}
	}

	void GstSound::Pause()
	{
		if (this->state == PLAYING)
		{
			this->state = PAUSED;
			gst_element_set_state(this->pipeline, GST_STATE_PAUSED);
		}
	}

	void GstSound::Stop()
	{
		if (this->state == PAUSED || this->state == PLAYING)
		{
			this->state = STOPPED;
			gst_element_set_state(this->pipeline, GST_STATE_NULL);
			GstSeekFlags flags
				 = (GstSeekFlags) (GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT);
			gst_element_seek_simple(
				this->pipeline,
				GST_FORMAT_TIME,
				flags,
				0);
		}
	}

	void GstSound::Reload()
	{
		this->Stop();
		this->Load();
	}

	void GstSound::SetVolume(double volume)
	{
		volume = volume * 10;
		g_object_set(G_OBJECT(this->pipeline), "volume", volume, NULL);
	}

	double GstSound::GetVolume()
	{
		gdouble volume;
		g_object_get(this->pipeline, "volume", &volume, NULL);
		return volume / 10;
	}

	void GstSound::SetLooping(bool looping)
	{
		this->looping = looping;
	}

	bool GstSound::IsLooping()
	{
		return this->looping;
	}

	bool GstSound::IsPlaying()
	{
		return this->state == PLAYING;
	}

	bool GstSound::IsPaused()
	{
		return this->state == PAUSED;
	}

	void GstSound::OnComplete(SharedBoundMethod callback)
	{
		this->callback = callback;
	}

	gboolean gst_bus_cb(GstBus *bus, GstMessage *message, gpointer data)
	{
		GstSound* sound = (GstSound*) data;
		if (message->type == GST_MESSAGE_ERROR)
		{
			sound->Stop();
		}
		else if (message->type == GST_MESSAGE_EOS)
		{
			sound->Stop();
			if (sound->IsLooping())
				sound->Play();
		}

		return true;
	}
}
