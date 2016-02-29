
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
    sim_window = perspective_window;
    sim_window2 = sim_window;

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
