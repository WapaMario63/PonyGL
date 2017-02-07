#ifndef ACT_H
#define ACT_H

/*
 * PoneNgine Acts
 *
 * PoneNgine Acts functions similar to a PoneNgine Program, but only contain
 * data of a stage, such as static models, entities, events, etc.
 *
 * Basically like a map for a game.
 *
 * Act files use the extension: .pact (Ponengine ACT file)
 *
 * Act's names always follow this name format:
 *      - game_gamemode-type_actname.pact
 *      - e.g. loeu_twn_ponydale_north, fw_plat_elyunke
 * This way, the engine can determine what game this act comes from and what
 * gamemode it should load. The default gamemodes for PoneNgine are as follows:
 *      - plat = Super Mario 64 style 3D platformer
 *      - twn = hub map or mmo rpg map. Twn means town stage.
 *      - spd or day = Sonic Unleashed/Generations like day time stage mode.
 *      - fgt or nit = Sonic Unleashed like beat-em-up stage, without the stretching.
 *      - fps = first person shooter, self explanatory.
 * Keep in mind these can be overriden midgame or during loading with console
 * commands.
 *
 * Like a PoneNgine Program, if they crash, they wont crash the game, though
 * it is kind of unlikely unless a static entity has some issues processing,
 * as all other entities are PoneNgine Programs themselves that can crash and
 * not affect all other Programs, and in this case, Act.
 *
 * Only one act can be loaded and rendered at a time, but you can load an Act
 * in the background and load it when needed, so long as you start unloading
 * the last Act.
 *
 * Acts have an entity limit of a maximun of a 32bit integer (2,147,483,647).
 * This is quite a lot and one won't see an Act going anywhere near that
 * limit (unless you are one of those guys).
 *
 * The following entities don't count towards the Act entity limit:
 *      - Players
 *          - The server has its own limit for this, maximun of a 16bit integer
 *            (65535) allowed.
 *      - Particle Systems attached to other entities like weapons and cosmetics.
 *      - Lights
 *      - Cameras
 *      - Option Entities placed in PoneNgine Studio
 *          - Things like Code and logic entities are hidden from view outside
 *            PoneNgine Studio and the Debug mode, in other words, hidden from
 *            normal game view.
 *
 * Acts can also be stored inside a .pmv (Ponengine Media Video) for Virtual Reality
 * Videos and Films done within PoneNgine Filmmaker and PoneNgine Funbox, though,
 * the filesize may be bigger than usual, depending of how good the compression is,
 * so it is mostly a sacrifice for better quality and immersion for the viewer.
 *
 */

#include "pne_utils.h"
#include <list>
#include "Graphics/ModelLoader.h"
#include "Graphics/Light.h"

class Act
{
public:
    Act();



private:
    int id;
    std::string name;
    std::string gamemode;
    std::string description;
    GLuint _fbo;
    GLuint _rbo;
    std::string ActDir;

    // TODO: Entity Storage and loaders
    //e.g. models and entity vectors (std::vector<Terrain>, std::vector<Entitiy>, etc.)
    std::list<ModelLoader> models;
    std::list<Light> lights;

    GLuint generateAttachmentTexture(bool depth, bool stencil);
    void GenerateFramebuffers();

};

#endif // ACT_H
