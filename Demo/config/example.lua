
sim_window = {
  view = {
    simulator_view = true;
  };
fullscreen = true;
};

perspective_window = {
  view = {
    eyes = {
      eye = cavr.sixdof("emulated");
      --left_eye = cavr.sixdof("vrpn[TallGlasses[[0]]");
      --right_eye = cavr.sixdof("vrpn[TallGlasses[[0]]");
      --stereo ="mono";
    };
    lower_left = cavr.sixdof("emulated") * cavr.translate(-1, -1, -1);
    lower_right = cavr.sixdof("emulated") * cavr.translate(1, -1, -1);
    upper_left = cavr.sixdof("emulated") * cavr.translate(-1, 1, -1);
  };
  fullscreen = true;
};

x11_renderer = {
  type = "x11gl";
  display = ":0.0";
  windows = {
    sim_window = perspective_window;
    sim_window2 = sim_window;

  };
};

x11_renderer_projector1 = {
  type = "x11gl";
  display = ":0.1";
  windows = {
    sim_window = perspective_window;
    --sim_window2 = sim_window;
  };
};

x11_renderer_projector2 = {
  type = "x11gl";
  display = ":0.2";
  windows = {
    sim_window = perspective_window;
    --sim_window2 = sim_window;
  };
};

vrpn = {
  type = "vrpn";
  input_name = "vrpn";
  buttons = {
    "WiiMote0@projector.cse.unr.edu";
  };
  analogs = {
  };
  sixdofs = {
    "TallGlasses@tracker.rd.unr.edu";
    "WiiMote0@tracker.rd.unr.edu";
  };
};

self = {
  hostname = "hpcvis6";
  ssh = "hpcvis6.cse.unr.edu";
  address = "hpcvis6.cse.unr.edu";
  plugins = {
    x11_renderer = x11_renderer;
    vrpn = vrpn;
  };
};

localhost = {
  hostname = "localhost";
  ssh = "localhost";
  address = "localhost";
  plugins = {
    x11_renderer = x11_renderer;
    vrpn = vrpn;
  };
};



others = {
  hostname = "hpcvis2";
  ssh = "hpcvis2";
  address = "hpcvis2";--"tcp://" .. "hpcvis7" .. ":8888";
  plugins = {
    x11_renderer = x11_renderer;
    vrpn = vrpn;
  };
  buttons = {
    "WiiMote0@tracker.rd.unr.edu";
  };
};

others2 = {
  hostname = "projector";
  ssh = "projector.cse.unr.edu";
  address = "projector.cse.unr.edu";--"tcp://" .. "hpcvis7" .. ":8888";
  plugins = {
    x11_top = x11_renderer_projector1;
    x11_bottom = x11_renderer_projector2;
    vrpn = vrpn;
  };
};


machines = {
  --self=self;
  --self2 = others;
  self = others2;
};
