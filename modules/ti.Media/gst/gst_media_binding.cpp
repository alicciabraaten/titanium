/**
 * Appcelerator Kroll - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2008 Appcelerator, Inc. All Rights Reserved.
 */
#include "gst_media_binding.h"

namespace ti
{
	GstMediaBinding::GstMediaBinding(SharedBoundObject global) : MediaBinding(global)
	{
		char **argv;
		int argc = 0;
		gst_init(&argc, &argv);
	}

	GstMediaBinding::~GstMediaBinding()
	{
	}

	void GstMediaBinding::Beep()
	{
		printf("\a"); // \a to console should cause the beep sound
	}

	SharedBoundObject GstMediaBinding::CreateSound(std::string& url)
	{
		std::string url_or_path = this->GetResourcePath(url.c_str());
		// This is a path so, turn it into a file:// URL
		if (url_or_path.find(':') == std::string::npos)
		{
			url_or_path = std::string("file://") + url_or_path;
		}
		return new GstSound(url_or_path);
	}
}
