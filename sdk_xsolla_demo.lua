
-- xsolla sdk交互demo

xsollaSdkDemo = xsollaSdkDemo or {}

-- https://developers.xsolla.com/zh/api/login/operation/generate-jwt/
-- 登录前获取token
xsollaSdkDemo.getJWT = function(client_id, client_secret, getAccessTokenUrl, callback)    
    local function onToken(opt, data)
        if opt ~= true then
            message("失败1") return 
        end
		local ok, tb = pcall(function() return json.decode(data) end)
		if not ok then 
			message("失败2") return
		end
        local accessToken = tb.access_token
        if accessToken == nil then
            message("失败3") return
        end
        dump(tb, "获取登录accessToken：", 6)
        if type(callback) == "function" then
            callback(accessToken)
        end
    end
    -- client_id， client_secret 在后台项目-登录管理器-创建标准登录管理器-进入配置-安全性(OAuth 2.0)-添加OAuth 2.0客户端(选择"服务器 (服务器对服务器连接)")-得到客户端id和秘钥
    local tb = {grant_type = "client_credentials", client_id = client_id, client_secret = client_secret}
    -- postData = true, 此处发送post数据使用&连接k=v数据
	local reqData = {url = getAccessTokenUrl, type = "POST", data = tb, postData = true, callback = onToken}
    -- http header
    local tbHeaderData = {["Content-Type"] = "application/x-www-form-urlencoded"}
    gameDef.sendHttp(reqData, nil, nil, nil, tbHeaderData)
end

-- https://developers.xsolla.com/zh/api/login/operation/auth-by-custom-id/
-- 通过指定要ID登录xsolla
xsollaSdkDemo.loginByCustomId = function(accessToken, reqUrl, server_custom_id, callback)    
    local function onToken(opt, data)
        if opt ~= true then
            message("失败1") return 
        end
		local ok, tb = pcall(function() return json.decode(data) end)
		if not ok then 
			message("失败2") return
		end
        local token = tb.token
        if token == nil then
            message("失败3") return
        end
        dump(tb, "获取loginByCustomId-token：", 6)
        if type(callback) == "function" then
            callback(token)
        end
    end
    -- 用户登录自定义ID(一般是服务端生成的用户ID)
    local tb = {server_custom_id = server_custom_id}
    -- postData = false, 此处发送post数据使用json字符串
	local reqData = {url = reqUrl, type = "POST", data = tb, postData = false, callback = onToken}
    -- http header
    local tbHeaderData = {["Content-Type"] = "application/json", ["X-SERVER-AUTHORIZATION"] = accessToken}
    gameDef.sendHttp(reqData, nil, nil, nil, tbHeaderData)
end

-- https://developers.xsolla.com/zh/api/igs-bb/operation/create-order-with-item/
-- 使用指定商品创建订单
xsollaSdkDemo.createOrder = function(reqUrl, bearerToken, itemSku, quantity, currency, bSandbox, custom_parameters, callback)   
    local function onPay(opt, data)
        if opt ~= true then
            message("支付失败1") return 
        end
		local ok, tb = pcall(function() return json.decode(data) end)
		if not ok then 
			message("支付失败2") return
		end
        local token = tb.token
        if token == nil then
            message("支付失败3") return
        end
        dump(tb, "获取支付token：", 6)
        if type(callback) == "function" then
            callback(token)
        end
    end
    local tb = {quantity = quantity, sandbox = bSandbox, currency = currency, custom_parameters = custom_parameters}
    -- postData = false, 此处发送post数据使用json字符串
	local reqData = {url = reqUrl, type = "POST", data = tb, postData = false, callback = onPay}
    local tbHeaderData = {["Content-Type"] = "application/json", ["Authorization"] = "Bearer "..bearerToken}
    gameDef.sendHttp(reqData, nil, nil, nil, tbHeaderData)
end


-- 打开支付UI
-- server_custom_id: 游戏id(一般为服务端生成的用户唯一ID)
-- itemSku: 物品sku，xsolla后台创建
-- quantity: 物品数量
-- currency: 货币类型, eg: "USD"
-- bSandbox: 是否是沙盒测试(false, true)
xsollaSdkDemo.pay = function(server_custom_id, itemSku, quantity, currency, bSandbox, callback) 
    -- 获取支付token
    local function onPayToken(payToken)
        local payUrl_production = "https://secure.xsolla.com/paystation4/?token="
        local payUrl_sandbox = "https://sandbox-secure.xsolla.com/paystation4/?token="
        local payUrl = (bSandbox == true) and payUrl_sandbox or payUrl_production
        local url = payUrl..payToken
		print("pay url:"..(url or ""))
        -- 打开支付界面
		cc.Application:getInstance():openURL(url)
        if type(callback) == "function" then
            callback(url)
        end
    end
    -- 登录成功返回token
    local function onBearerToken(bearerToken)
        -- 使用指定商品创建订单
        -- 自定义参数(购买成功后会回调到url参数)
        -- 支付后回到url在后台项目-项目设置-Webhooks下设置
        local custom_parameters = { str = "123654", }
        -- 这个项目的ID, 在后台选择对应的项目，项目名字下发就能看到ID xxx
        local projectId = "247911"
        local formatUrl = "https://store.xsolla.com/api/v2/project/%s/payment/item/%s"
        local reqUrl = string.format(formatUrl, projectId, itemSku)
        xsollaSdkDemo.createOrder(reqUrl, bearerToken, itemSku, quantity, currency, bSandbox, custom_parameters, onPayToken)   
    end
    -- 登录前获取token
    local function onAccessToken(accessToken)
        -- 通过指定要ID登录xsolla
        local formatUrl = "https://login.xsolla.com/api/users/login/server_custom_id?projectId=%s&payload=%s&publisher_project_id=%s&with_logout=%s"
        -- projectId 在后台项目-登录管理器-已经创建好的OAuth2.0-配置-复制ID
        local projectId = "1ec69e8c-c35f-4d30-92c7-08b9d31e1111"
        -- 自定义参数
        local payload = "joke_params"
        -- 这个项目的ID, 在后台选择对应的项目，项目名字下发就能看到ID xxx
        local publisher_project_id = "247111"
        -- 默认0
        local with_logout = "0"
        local reqUrl = string.format(formatUrl, projectId, payload, publisher_project_id, with_logout)
        xsollaSdkDemo.loginByCustomId(accessToken, reqUrl, server_custom_id, onBearerToken)   
    end
    -- client_id， client_secret 在后台项目-登录管理器-创建标准登录管理器-进入配置-安全性(OAuth 2.0)->
    -- 添加OAuth 2.0客户端(选择"服务器 (服务器对服务器连接)")-得到客户端id和秘钥
    local client_id = "7611"
    local client_secret = "Wjo4TpQoQmee8SdNOxRNT3pzRefV1111"
    local getAccessTokenUrl = "https://login.xsolla.com/api/oauth2/token"
    xsollaSdkDemo.getJWT(client_id, client_secret, getAccessTokenUrl, onAccessToken)   
end

-- 测试调用
--xsollaSdkDemo.pay("server_custom_id", "charge_id_3", 1, "USD", true) 