sim_window = {
  view = {
    simulator_view = true;
  };
fullscreen = true;
};

sim_window = {
  view = {
    simulator_view = true;
  };
fullscreen = true;
};

perspective_window = {
  view = {
    eyes = {
      eye = cavr.sixdof("TallGlass");
      --left_eye = cavr.sixdof("emulated3");
      --right_eye = cavr.sixdof("emulated2");
      --stereo ="mono";
    };
    lower_left = cavr.sixdof("TallGlass") * cavr.translate(-1, -1, -1);
    lower_right = cavr.sixdof("TallGlass") * cavr.translate(1, -1, -1);
    upper_left = cavr.sixdof("TallGlass") * cavr.translate(-1, 1, -1);
  };
  fullscreen = true;
};

x11_renderer = {
  type = "x11gl";
  display = ":1.0";
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
    --"Button0@localhost";
  };
  analogs = {
  };
  sixdofs = {
    "TallGlass@tracker.rd.unr.edu"
  };
};

self = {
  hostname = "hpcvis1";
  ssh = "hpcvis1.cse.unr.edu";
  address = "hpcvis1";
  plugins = {
    x11_renderer = x11_renderer;
    vrpn = vrpn;
  };
};


others = {
  hostname = "hpcvis4";
  ssh = "hpcvis4";
  address = "hpcvis4";--"tcp://" .. "hpcvis7" .. ":8888";
  plugins = {
    x11_renderer = x11_renderer;
    vrpn = vrpn;
  };
};

others2 = {
  hostname = "projector";
  ssh = "projector.cse.unr.edu";
  address = "projector";--"tcp://" .. "hpcvis7" .. ":8888";
  plugins = {
    x11_renderer1 = x11_renderer_projector1;
    x11_renderer2 = x11_renderer_projector2;
    vrpn = vrpn;
  };
};


machines = {
  self=self;
  --self2 = others;
  self3 = others2;
};
