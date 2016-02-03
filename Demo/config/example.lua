
sim_window = {
  view = {
    simulator_view = true;
  };
fullscreen = true;
};

perspective_window = {
  view = {
    eyes = {
      eye = cavr.sixdof("vrpn[TallGlass[0]]");
      --left_eye = cavr.sixdof("vrpn[TallGlass[[0]]");
      --right_eye = cavr.sixdof("vrpn[TallGlass[[0]]");
      --stereo ="mono";
    };
    lower_left = cavr.sixdof("vrpn[TallGlass[0]]") * cavr.translate(-1, -1, -1);
    lower_right = cavr.sixdof("vrpn[TallGlass[0]]") * cavr.translate(1, -1, -1);
    upper_left = cavr.sixdof("vrpn[TallGlass[0]]") * cavr.translate(-1, 1, -1);
  };
  fullscreen = true;
};

x11_renderer = {
  type = "x11gl";
  display = ":0.0";
  windows = {
    --sim_window = perspective_window;
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
    "WiiMote@tracker.rd.unr.edu";
  windows = {
    sim_window = perspective_window;
    --sim_window2 = sim_window;
  };
};

vrpn = {
  type = "vrpn";
  input_name = "vrpn";
  buttons = {
    --"WiiMote0@projector.unr.edu";
  };
  analogs = {
  };
  sixdofs = {
    "TallGlass@tracker.rd.unr.edu";
    "WiiMote@tracker.rd.unr.edu";
  };
};

self = {
  hostname = "hpcvis1";
  ssh = "hpcvis1.cse.unr.edu";
  address = "hpcvis1.cse.unr.edu";
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
    "WiiMote@tracker.rd.unr.edu";
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
  self=self;
  --self2 = others;
  --self3 = others2;
};
