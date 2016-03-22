
sim_window = {
  view = {
    simulator_view = true;
  };
fullscreen = false;
};

-- Use a emulated a sixdof
perspective_window = {
  view = {
    eyes = {
      eye = cavr.sixdof("emulated");
      --left_eye = cavr.sixdof("vrpn[TallGlass[[0]]");
      --right_eye = cavr.sixdof("vrpn[TallGlass[[0]]");
      --stereo ="mono";
    };
    lower_left = cavr.sixdof("emulated") * cavr.translate(-1, -1, -1);
    lower_right = cavr.sixdof("emulated") * cavr.translate(1, -1, -1);
    upper_left = cavr.sixdof("emulated") * cavr.translate(-1, 1, -1);
  };
  fullscreen = true;
  far = 100000.0;
};

x11_renderer = {
  type = "x11gl";
  display = ":1.0";
  windows = {
    sim_window = perspective_window;
    sim_window2 = sim_window;

  };
};


vrpn = {
  type = "vrpn";
  input_name = "vrpn";
  buttons = {
    "WiiMote0@projector.cse.unr.edu";
  };
  analogs = {
    "WiiMote0@projector.cse.unr.edu";
  };
  sixdofs = {
    "ShortGlasses@tracker.rd.unr.edu:1";
    "WiiMote0@tracker.rd.unr.edu:1";
  };
};

self = {
  hostname = HOSTNAME;
  ssh = HOSTNAME;
  address = HOSTNAME;
  plugins = {
    x11_renderer = x11_renderer;
    vrpn = vrpn;
  };
};

machines = {
  self=self;
};
