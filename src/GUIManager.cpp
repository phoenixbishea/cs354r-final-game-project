#include "GUIManager.hpp"




GUIManager::GUIManager(Larp::SceneGraphPtr g):
_graph(g)
{
	this->_renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();
	this->_wmgr =  &CEGUI::WindowManager::getSingleton();
	// this->_renderer->setupExtraStates();
	// this->_renderer->enableExtraStateSettings(true);

	this->setup_resources();
	this->setup_main_menu();
}

GUIManager::~GUIManager()
{

}

void GUIManager::setup_resources()
{
	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
   (CEGUI::System::getSingleton().getResourceProvider());


  rp->setResourceGroupDirectory("schemes", "assets/schemes/");
	rp->setResourceGroupDirectory("imagesets", "assets/imagesets/");
	rp->setResourceGroupDirectory("fonts", "assets/fonts/");
	rp->setResourceGroupDirectory("layouts", "assets/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "assets/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "assets/lua_scripts/");

	/* Setups all the resources */
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

	/* Using the TahrezLook since it is prebuilt. May change for custom gui */
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
	CEGUI::FontManager::getSingleton().createFromFile( "DejaVuSans-10.font" );


}

void GUIManager::setup_main_menu()
{
	CEGUI::Window* mainSheet = this->_wmgr->createWindow("DefaultWindow", "CEGUIDemo/mainSheet");

	CEGUI::Window* addLight = this->_wmgr->createWindow("TaharezLook/Button", "Game/addLight");
	CEGUI::Window* lightMenu = this->_wmgr->createWindow("TaharezLook/Menubar", "lightMenu");

	CEGUI::Window* pointLight = this->_wmgr->createWindow("TaharezLook/MenuItem", "point");
	CEGUI::Window* spotLight = this->_wmgr->createWindow("TaharezLook/MenuItem", "spot");
	CEGUI::Window* directionalLight = this->_wmgr->createWindow("TaharezLook/MenuItem", "directional");




	pointLight->setText("Point Light");
	spotLight->setText("Spotlight");
	directionalLight->setText("Directional Light");

  lightMenu->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4f,0),CEGUI::UDim(0.4f,0)));
  lightMenu->setSize(CEGUI::USize(CEGUI::UDim(0.25,0), CEGUI::UDim(0.05,0)));

  addLight->setText("Add Light...");
  addLight->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05f,0),CEGUI::UDim(0.2f,0)));
  addLight->setSize(CEGUI::USize(CEGUI::UDim(0.10,0), CEGUI::UDim(0.05,0)));

  addLight->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::add_light, this));
  pointLight->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::add_point_light, this));
  spotLight->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::add_spot_light, this));

  lightMenu->addChild(pointLight);
  lightMenu->addChild(spotLight);
  lightMenu->addChild(directionalLight);


  mainSheet->addChild(addLight);
  // popupMenu->addChild(start);

  this->_sheets.push_back(mainSheet);
  this->_sheets.push_back(lightMenu);

  CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(mainSheet);
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
}

bool GUIManager::get_rendering_state()
{
	return this->_GUI_rendering;
}

void GUIManager::add_light(const CEGUI::EventArgs&)
{
		this->_sheets.at(MAIN)->addChild(this->_sheets.at(ADDLIGHT));
}

void GUIManager::add_point_light(const CEGUI::EventArgs&)
{

	this->_sheets.at(MAIN)->removeChild("lightMenu");
	this->hide_GUI();
	Larp::PointLightPtr point_light = this->_graph->create_point_light();

	point_light->set_ambient_intensity(0.0f, 1.0f, 0.0f);
  point_light->set_position(0.0f, 2.0f, 0.0f);

}

void GUIManager::add_spot_light(const CEGUI::EventArgs&)
{

	this->_sheets.at(MAIN)->removeChild("lightMenu");
	this->hide_GUI();

	Larp::SpotLightPtr spot_light = this->_graph->create_spot_light();

	spot_light->set_ambient_intensity(0.0f, 1.0f, 0.0f);
 	spot_light->set_position(0.0f, 2.0f, 0.0f);

}

void GUIManager::add_directional_light(const CEGUI::EventArgs&)
{

	this->_sheets.at(MAIN)->removeChild("lightMenu");
	this->hide_GUI();

	Larp::DirectionalLightPtr directional_light = this->_graph->create_directional_light();

	// directional_light->set_ambient_intensity(0.0f, 1.0f, 0.0f);
 // 	directional_light->set_position(0.0f, 2.0f, 0.0f);

}

void GUIManager::hide_GUI()
{
	this->_sheets.at(MAIN)->hide();
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
	this->_GUI_rendering = false;
}

void GUIManager::show_GUI()
{
		this->_sheets.at(MAIN)->show();
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
		this->_GUI_rendering = true;
}

void GUIManager::show_main()
{
	if(_GUI_rendering)
		this->hide_GUI();
	else
		this->show_GUI();

}