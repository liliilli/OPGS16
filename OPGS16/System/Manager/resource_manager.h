#ifndef OPGS16_MANAGER_RESOURCE_MANAGER_H
#define OPGS16_MANAGER_RESOURCE_MANAGER_H

/**
 * @file System\Manager\resource_manager.h
 * @author Jongmin Yun
 * @date 2018-02-28
 *
 * @log
 * 2018-02-27 Correction of loading mechanism
 * 2018-02-28 Corrected GetTexture2D return type must be resource::Texture2D structure.
 */

#include <string>				/** std::string */
#include <utility>				/** std::pair */
#include <unordered_map>		/** std::unordered_map */
#include "resource_type.h"      /*! resource:: */
#include "..\Shader\shader.h"

/**
 * @class ResourceManager
 * @brief This class is singleton and not derivable as derived class.
 * ResourceManager has a rule of managing resource files path, each shader path,
 * , error statement path and so on.
 *
 * ResrouceManager has to be called (invoked) when game application is setting up, before
 * actual game update has been begun.
 *
 * @date 2018-02-28
 *
 * @log
 * 2018-02-27 Correction of loading mechanism
 * 2018-02-28 Corrected GetTexture2D return type must be resource::Texture2D structure.
 * @todo CheckError method must have a procedure to bring error message display call to logger.
 */
class ResourceManager final {
public:
	/** Internal type aliasings. */
	using shader_type       = helper::ShaderNew::Type;
	using shader_pair       = std::pair<shader_type, const std::string>;
	using shader_list       = std::vector<shader_pair>;

    /*! Resource map list */
    using texture_map       = std::unordered_map<std::string, resource::Texture2D>;
    using sound_map         = std::unordered_map<std::string, std::string>;
    using shader_map        = std::unordered_map<std::string, shader_list>;

public:
	/**
	 * @brief Return single static instance. This must be called in initiation time once.
	 * @return The reference of ResourceManager instance.
	 */
	static ResourceManager& GetInstance() {
		static ResourceManager instance{};
		return instance;
	}

    /*! Empty already allocated resources and load resource with file path. */
    bool LoadResource(const std::string& path);

	/**
	 * @brief Get specific shader list.
	 * @param[in] name_key shader program aliasing name.
	 * @return Specific shader container.
	 */
	shader_list& GetShader(const std::string& name_key);

	/**
	 * @brief Get specific texture path.
	 * @param[in] name_key Wrapping tag name of actual sprite path.
	 * @return Selected 2D texture path.
	 */
	const resource::Texture2D& GetTexture2D(const std::string& name_key);

    /*!
     * @brief Get specified sound path information.
     * @param[in] name_key Wrapping tag name of actual sound path.
     * @return Sound path
     */
    const std::string& GetSound(const std::string& name_key);

	/*! Check error caused by processing in this class instance. */
	void CheckError();

private:
	texture_map m_textures;         /*! Texture string container */
	shader_map  m_shaders;          /*! Shader arguments container */
    sound_map   m_sounds;           /*! Sounds path string container */

	/** Used for checking error invoked when this methods process something. */
	enum class ErrorType {
		OK,							/** Default type, everything is ok. */
		FAILED_INITIALIZE_SHADER,	/** Invoked when failed to input shader information. */
		FAILED_INITIALIZE_TEXTURE2D,/** Invoked when failed to input texture information. */
	} m_error{ ErrorType::OK };

    /*! Used for checking symbol type to read. */
    enum class SymbolType {
        NOTHING,                    /*! Default value of symbol type */
        RESOURCE,                   /*! Resource symbol. */
        GLOBAL_PATH,                /*! Global path symbol to read from. */
        COMMENT,                    /*! Comment symbol. */
    };

    enum class ResourceType;

    const std::string TEX2D = "TEX2D";
    const std::string SHADE = "SHADE";
    const std::string SEBGM = "SEBGM";

private:
    /*! Insert already templated resource code. */
    void InsertSampleResources();

    /*! Read symbol and return with token string. */
    std::pair<SymbolType, std::string> ReadSymbol(const std::string& stream);

    /*! Get resource type from token. */
    ResourceType GetResourceType(const std::string& resource_token);

    /*! Read resource along with resource_type */
    void ReadResource(const std::string& token_line,
                      std::ifstream& stream,
                      const std::string& global_path,
                      ResourceType type);

    void ReadTexture2D(std::ifstream& stream, const std::string& global_path);


    /*! Read shader path and return this as a vector container. */
    std::vector<shader_pair> ReadShaderPath(std::ifstream& stream,
                                            const std::string& global_path);

	/**
	 * @brief In initialization time of game application, push shader information to container.
	 * This method must be called at this time. otherwise method refuse processing.
	 * If name_key is duplicated, initialization halts and exit game with message.
	 *
	 * param[in] name_key shader program's aliasing name.
	 * param[in] list Initialize list has a pair of pipeline type and shader code path.
	 */
	void PushShader(const std::string& name_key, const shader_list& list);

	/**
	 * @brief In initialization time of game application, insert texture_2d information
	 * to container. This method must be called at initialzation time. otherwise method refuse
	 * processing.
	 * If name_key is duplicated, initialization halts and exit game with a message.
	 *
	 * param[in] name_key sprite path's wrapping name.
	 * param[in] path String path to read file when called.
	 */
	void PushTexture2D(const std::string& name_key, const resource::Texture2D& container);

    /*! In initialization of game application or scene loading, insert sound path information. */
    void PushSound(const std::string& name_key, const std::string& path);

	/**
	 * @brief Check whether or not shader has name_key already exists.
	 * In runtime game mechanism, this must not be happened but if happened, you have to make
	 * an log in debug mode.
     *
	 * @param[in] name_key The key to find.
	 * @return boolean value, If duplicated return true else false.
	 */
	inline bool ExistShaderKey(const std::string& name_key) {
		return m_shaders.find(name_key) != m_shaders.end();
	}

	/**
	 * @brief Check whether or not texture has name_key already exists.
	 * In runtime game mechanism, this must not be happened but if happened, you have to make
	 * an log in debug mode.
     *
	 * @param[in] name_key The key to find.
	 * @return boolean value, If exist return true else false.
	 */
	inline bool ExistTextureKey(const std::string& name_key) {
		return m_textures.find(name_key) != m_textures.end();
	}

    /*! If whether or not key already exist in sound resource path container. */
    inline bool ExistSoundKey(const std::string& name_key) {
        return m_sounds.find(name_key) != m_sounds.end();
    }

private:
    ResourceManager();

public:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(const ResourceManager&&) = delete;
};

#endif /** OPGS16_MANAGER_RESOURCE_MANAGER_H */