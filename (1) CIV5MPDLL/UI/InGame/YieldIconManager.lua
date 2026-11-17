print("This is the modded YieldIconManager from 'DLL - Various Mod Components'")

-------------------------------------------------
-- Yield Icon Manager
-------------------------------------------------
include("FLuaVector");
include("InstanceManager");

-- CameraView tracks the viewport and fires PlotEnterView/PlotLeaveView.
-- It is optional: a DLC that missed the file falls back to the engine's
-- Events.ShowHexYield below. The flag proves the include ran to the end.
local bHasCameraView = false
local bCameraViewLoaded, tCameraViewError = pcall(include, "CameraView")
if bCameraViewLoaded then
    bHasCameraView = (MPDLL_CAMERA_VIEW_LOADED == true)
    if not bHasCameraView then
        print("YieldIconManager: CameraView loaded but did not set MPDLL_CAMERA_VIEW_LOADED")
    end
else
    print("YieldIconManager: CameraView failed to load (" .. tostring(tCameraViewError) .. ")")
end

local g_AnchorIM = InstanceManager:new("AnchorInstance", "Anchor", Controls.Scrap);
local g_ImageIM  = InstanceManager:new("ImageInstance",  "Image",  Controls.Scrap);

local g_VisibleSet = {};
local g_YieldInfoCache = {};

-- This MUST have (at least) YieldTypes.NUM_YIELD_TYPES entries!  (There are currently a few spare)
local g_StackPlacement = {1, 1, 1, 2, 2, 2, 1, 2, 1, 2}

--
-- EUI (possibly any pseudo-DLC) is exposing a weirdness in the way the GameInfo cache works
--
-- Without EUI
-- [ Lua State = YieldIconManager ]
-- > GameInfo.Yields[1].ImageTexture
--  YieldAtlas.dds
-- > for row in DB.Query("SELECT ImageTexture FROM Yields WHERE ID=1") do print(row.ImageTexture) end
--  YieldIconManager: YieldAtlas.dds
--
-- With EUI
-- [ Lua State = YieldIconManager ]
-- > GameInfo.Yields[1].ImageTexture
--  YieldIconManager: _cmdr = {GameInfo.Yields[1].ImageTexture}:1:  Cannot find key - ImageTexture
-- > for row in DB.Query("SELECT ImageTexture FROM Yields WHERE ID=1") do print(row.ImageTexture) end
--  YieldIconManager: YieldAtlas.dds
--
-- So we'll work around it
--
local g_YieldTextures = {"YieldAtlas.dds", "YieldAtlas.dds", "YieldAtlas.dds", "YieldAtlas.dds", "YieldAtlas_128_Culture.dds", "YieldAtlas_128_Faith.dds"}
local g_YieldOffsets = {0, 128, 256, 384, 0, 0}

for row in DB.Query("SELECT * FROM Yields") do
  for col, val in pairs(row) do
    if (col == "ImageTexture") then
	  g_YieldTextures[row.ID+1] = val
	elseif (col == "ImageOffset") then
	  g_YieldOffsets[row.ID+1] = val
	end
  end
end


------------------------------------------------------------------
-- update the controls to reflect the current known yield
------------------------------------------------------------------
function BuildYield(gridX, gridY, index, pPlot)
    if (not pPlot:IsRevealed(Game.GetActiveTeam(), false)) then
        return;
    end

    local yieldInfo = g_YieldInfoCache[index];
    if (yieldInfo == nil) then
        yieldInfo = RecordYieldInfo(index, pPlot);

        if (yieldInfo == nil) then
            return;
        end
    end

    record = {};
    record.AnchorInstance = g_AnchorIM:GetInstance();
    record.AnchorInstance.Anchor:ChangeParent(Controls.YieldStore);
    g_VisibleSet[index] = record;
    local anchor = record.AnchorInstance;

	local iPlace = 1
	for iYield = 0, YieldTypes.NUM_YIELD_TYPES-1, 1 do
		if (yieldInfo[iYield] > 0) then
			SetYieldIcon(iYield, yieldInfo[iYield], anchor[string.format("Stack%i", (g_StackPlacement[iPlace] or 1))], record);
			iPlace = iPlace + 1;
		end
	end

    anchor.Stack1:CalculateSize();
    anchor.Stack1:ReprocessAnchoring();
    anchor.Stack2:CalculateSize();
    anchor.Stack2:ReprocessAnchoring();
    anchor.Anchor:SetHexPosition(gridX, gridY);
end

------------------------------------------------------------------
function SetYieldIcon(yieldType, amount, parent, record)
    local imageInstance = g_ImageIM:GetInstance();
	imageInstance.Image:SetTexture(g_YieldTextures[yieldType+1]);

	local imageOffset = g_YieldOffsets[yieldType+1]
    if (amount > 5) then
        imageInstance.Image:SetTextureOffsetVal(imageOffset, 512);
    else
        imageInstance.Image:SetTextureOffsetVal(imageOffset, 128 * (amount - 1));
    end

	imageInstance.Image:ChangeParent(parent);

	if (record.ImageInstances == nil) then
		record.ImageInstances = {};
	end
	table.insert(record.ImageInstances, imageInstance);

	if (amount > 5) then
		local textImageInstance = g_ImageIM:GetInstance();
		textImageInstance.Image:SetTextureOffsetVal(GetNumberOffset(amount));
		textImageInstance.Image:ChangeParent(imageInstance.Image);
		table.insert(record.ImageInstances, textImageInstance);
    end
end

------------------------------------------------------------------
function GetNumberOffset(number)
    if (number > 12) then
        number = 12;
    end

    local x = 128 * ((number - 6) % 4);

    local y;
    if (number > 9) then
        y = 768
    else
        y = 640;
    end

    return x, y;
end

------------------------------------------------------------------
function DestroyYield(index)
    local instanceSet = g_VisibleSet[index];

    if (instanceSet) then
        local instance = instanceSet.AnchorInstance;

        if (instance) then
            instance.Anchor:ChangeParent(Controls.Scrap);
            g_AnchorIM:ReleaseInstance(instance);
        end

        for _, instance in ipairs(instanceSet.ImageInstances) do
            instance.Image:ChangeParent(Controls.Scrap);
            g_ImageIM:ReleaseInstance(instance);
        end

        g_VisibleSet[index] = nil;
    end
end

------------------------------------------------------------------
function RecordYieldInfo(index, pPlot)
    local info = {};
    local iTotal = 0

	for iYield = 0, YieldTypes.NUM_YIELD_TYPES-1, 1 do
        info[iYield] = pPlot:CalculateYield(iYield, true);
		iTotal = iTotal + info[iYield]
	end

    if (iTotal == 0) then
        info = nil;
    end

    g_YieldInfoCache[index] = info;

    return info;
end

-----------------------------------------------------------------------
function ShowHexYield(gridX, gridY, bShow)
    local pPlot = Map.GetPlot(gridX, gridY);

    if (pPlot) then
		local index = pPlot:GetPlotIndex();

		if (bShow) then
			DestroyYield(index);
			BuildYield(gridX, gridY, index, pPlot);
		else
			DestroyYield(index);
		end
	end

end

----------------------------------------------------------------
-- Viewport-driven mode
--
-- CameraView (when present) tells us which plots the camera can see, and
-- the engine's C++ YieldIconManager is unregistered so it does not do the
-- same work twice. The engine also owns RequestYieldDisplay, which is how
-- the city screen and the worker/settler selection show their plots; the
-- Lua side below takes that over once the engine manager is off.
----------------------------------------------------------------
local CALLER = {}

local g_ViewSet = {}
local g_FilterFn = nil
local g_PersistSet = nil
local bGlobalOn = true

-- Map.GetPlot does not wrap, but AREA requests near the seam do
local gMapWrapX = Map.IsWrapX()
local gMapWrapY = Map.IsWrapY()
local gMapWidth, gMapHeight = Map.GetGridSize()

local function GetPlotWrapped(gridX, gridY)
    if gMapWrapX then
        gridX = gridX % gMapWidth
    end
    if gMapWrapY then
        gridY = gridY % gMapHeight
    end
    return Map.GetPlot(gridX, gridY)
end

local bExeDisabled = false
if bHasCameraView then
    if Exe and Exe.TryDisableEngineYieldIconManager then
        local bOK, szReason = Exe.TryDisableEngineYieldIconManager()
        bExeDisabled = (bOK == true)
        if not bExeDisabled then
            print("YieldIconManager: engine YieldIconManager not disabled (" .. tostring(szReason) .. ")")
        end
    else
        print("YieldIconManager: Exe API missing, keeping the engine yield manager")
    end
end

-- Both must hold. If the engine manager is still registered it keeps
-- emitting ShowHexYield, so taking over RequestYieldDisplay as well would
-- draw every display request twice.
local bUseCameraView = bHasCameraView and bExeDisabled

if bUseCameraView and OptionsManager and OptionsManager.GetYieldOn then
    bGlobalOn = OptionsManager.GetYieldOn() and true or false
end

-- One line per load so the active path can be checked from Lua.log.
print("YieldIconManager: viewport takeover " .. (bUseCameraView and "active" or "inactive")
    .. ", global=" .. tostring(bGlobalOn))

local function BuildByIndex(index)
    local pPlot = Map.GetPlotByIndex(index)
    if pPlot then
        BuildYield(pPlot:GetX(), pPlot:GetY(), index, pPlot)
    end
end

local function RenderPlot(index)
    DestroyYield(index)
    BuildByIndex(index)
end

local function DestroyAllVisible()
    local tKeys = {}
    for index in pairs(g_VisibleSet) do
        tKeys[#tKeys + 1] = index
    end
    for i = 1, #tKeys do
        DestroyYield(tKeys[i])
    end
end

-- Global "show yields" is the whole viewport. When it is off, only the
-- plots the UI asked for (city screen, worker selection, empire view)
-- are shown.
local function ViewFilterAllows(index)
    if bGlobalOn then
        return true
    end
    if g_FilterFn then
        return g_FilterFn(index) == true
    end
    if g_PersistSet then
        return g_PersistSet[index] == true
    end
    return false
end

local function RequestAll()
    LuaEvents.RequestViewPlots(CALLER)
end

-- Destroy icons outside the set and draw the set's plots that are in view
local function RenderSet(tSet)
    local tDrop = {}
    for index in pairs(g_VisibleSet) do
        if not tSet[index] then
            tDrop[#tDrop + 1] = index
        end
    end
    for i = 1, #tDrop do
        DestroyYield(tDrop[i])
    end

    for index in pairs(tSet) do
        if g_ViewSet[index] and not g_VisibleSet[index] then
            RenderPlot(index)
        end
    end
end

local function RenderConditional(pfnAllowed)
    local tDrop = {}
    for index in pairs(g_VisibleSet) do
        if not pfnAllowed(index) then
            tDrop[#tDrop + 1] = index
        end
    end
    for i = 1, #tDrop do
        DestroyYield(tDrop[i])
    end

    for index in pairs(g_ViewSet) do
        if pfnAllowed(index) and not g_VisibleSet[index] then
            RenderPlot(index)
        end
    end
end

local function RenderCurrent()
    if bGlobalOn then
        DestroyAllVisible()
        RequestAll()
        return
    end

    if g_FilterFn then
        RenderConditional(g_FilterFn)
    elseif g_PersistSet then
        RenderSet(g_PersistSet)
    else
        DestroyAllVisible()
    end
end

local function ClearFilter()
    g_FilterFn = nil
    g_PersistSet = nil
    RenderCurrent()
end

local function SetCityFilter(displayType, pCity)
    if pCity == nil then
        ClearFilter()
        return
    end

    local iOwner = pCity:GetOwner()
    local iCity = pCity:GetID()
    local tSet = {}

    for i = 0, pCity:GetNumCityPlots() - 1 do
        local pCityPlot = pCity:GetCityIndexPlot(i)

        if pCityPlot then
            local bAllowed = false

            if displayType == YieldDisplayTypes.CITY_OWNED then
                bAllowed = (pCityPlot:GetOwner() == iOwner)
            elseif displayType == YieldDisplayTypes.CITY_WORKED then
                local pWorkingCity = pCityPlot:GetWorkingCity()
                bAllowed = pWorkingCity ~= nil
                    and pWorkingCity:GetID() == iCity
                    and pWorkingCity:GetOwner() == iOwner
            elseif displayType == YieldDisplayTypes.CITY_PURCHASABLE then
                bAllowed = (pCityPlot:GetOwner() ~= iOwner)
            end

            if bAllowed then
                tSet[pCityPlot:GetPlotIndex()] = true
            end
        end
    end

    g_FilterFn = nil
    g_PersistSet = tSet
    RenderCurrent()
end

local function SetAreaFilter(radius, gridX, gridY)
    if radius == nil or radius <= 0 then
        ClearFilter()
        return
    end

    if gMapWrapX then
        gridX = gridX % gMapWidth
    end
    if gMapWrapY then
        gridY = gridY % gMapHeight
    end

    local tSet = {}

    -- Hex distance, not a square: this matches the radius the engine used
    for dy = -radius, radius do
        for dx = -radius, radius do
            local pPlot = GetPlotWrapped(gridX + dx, gridY + dy)
            if pPlot and Map.PlotDistance(gridX, gridY, pPlot:GetX(), pPlot:GetY()) <= radius then
                tSet[pPlot:GetPlotIndex()] = true
            end
        end
    end

    g_FilterFn = nil
    g_PersistSet = tSet
    RenderCurrent()
end

local function SetEmpireFilter()
    g_PersistSet = nil
    g_FilterFn = function(index)
        local pPlot = Map.GetPlotByIndex(index)
        return pPlot ~= nil and pPlot:GetOwner() == Game.GetActivePlayer()
    end
    RenderCurrent()
end

-- The engine's RequestYieldDisplay reaches Lua with unusable values in the
-- coordinate slots, so derive the city from the current selection and let
-- the option decide whether the unit area should be shown at all.
local function GetRequestCity(a, b)
    local pCity = UI.GetHeadSelectedCity()
    if pCity then
        return pCity
    end

    -- The arguments this event delivers are unreliable in this build (a is
    -- often a garbage integer and b is an empty table), so the selected city
    -- is the only dependable source. The coordinate lookup stays as a last
    -- resort in case the dispatch changes.
    if type(a) == "number" and type(b) == "number" then
        local pPlot = Map.GetPlot(a, b)
        if pPlot then
            return pPlot:GetPlotCity()
        end
    end

    return nil
end

local function OnRequestYieldDisplay(displayType, a, b, c)
    if displayType == YieldDisplayTypes.USER_ALL_ON then
        bGlobalOn = true
        ClearFilter()
    elseif displayType == YieldDisplayTypes.USER_ALL_OFF then
        bGlobalOn = false
        RenderCurrent()
    elseif displayType == YieldDisplayTypes.EMPIRE then
        SetEmpireFilter()
    elseif displayType == YieldDisplayTypes.AREA then
        -- InGame.lua sends (AREA, 2, x, y) only for a settler and (AREA, 0)
        -- for everything else (military units included). The numeric slots
        -- are unreliable, so use the same predicate the original UI used.
        local pUnit = UI.GetHeadSelectedUnit()
        local bCanFound = UI.CanSelectionListFound and UI.CanSelectionListFound()

        if pUnit and bCanFound and OptionsManager and OptionsManager.IsCivilianYields
            and OptionsManager.IsCivilianYields() then
            SetAreaFilter(2, pUnit:GetX(), pUnit:GetY())
        else
            ClearFilter()
        end
    elseif displayType == YieldDisplayTypes.CITY_OWNED
        or displayType == YieldDisplayTypes.CITY_WORKED
        or displayType == YieldDisplayTypes.CITY_PURCHASABLE then
        SetCityFilter(displayType, GetRequestCity(a, b))
    end
end

local function OnPlotEnterView(index)
    g_ViewSet[index] = true

    if ViewFilterAllows(index) then
        RenderPlot(index)
    end
end

local function OnPlotLeaveView(index)
    g_ViewSet[index] = nil
    DestroyYield(index)
end

local function OnHexFogEvent(hexPos, fowType, bWholeMap)
    if bWholeMap then
        for index in pairs(g_ViewSet) do
            if ViewFilterAllows(index) then
                RenderPlot(index)
            end
        end
        return
    end

    local gridX, gridY = ToGridFromHex(hexPos.x, hexPos.y)
    local pPlot = Map.GetPlot(gridX, gridY)

    if pPlot then
        local index = pPlot:GetPlotIndex()
        if g_ViewSet[index] and ViewFilterAllows(index) then
            RenderPlot(index)
        end
    end
end

-------------------------------------------------
function OnYieldChangeEvent(gridX, gridY)
    local pPlot = Map.GetPlot(gridX, gridY);

    if (pPlot == nil) then
        print("Missing Plot information [" .. gridX .. " " .. gridY .. "]");
        return;
    end

    if (not pPlot:IsRevealed(Game.GetActiveTeam(), false)) then
        return;
    end

    local index = pPlot:GetPlotIndex();

    RecordYieldInfo(index, pPlot);

    if (g_VisibleSet[index]) then
        DestroyYield(index);
        BuildYield(gridX, gridY, index, pPlot);
    end

end
Events.HexYieldMightHaveChanged.Add(OnYieldChangeEvent);

----------------------------------------------------------------
-- 'Active' (local human) player has changed
----------------------------------------------------------------
function OnActivePlayerChanged(iActivePlayer, iPrevActivePlayer)
	for index, _ in pairs(g_YieldInfoCache) do
        g_YieldInfoCache[index] = nil;
   	end

    if bUseCameraView then
        DestroyAllVisible()
        g_ViewSet = {}
        g_FilterFn = nil
        g_PersistSet = nil
        if OptionsManager and OptionsManager.GetYieldOn then
            bGlobalOn = OptionsManager.GetYieldOn() and true or false
        end
        RequestAll()
    else
        UI.RefreshYieldVisibleMode();
    end
end
Events.GameplaySetActivePlayer.Add(OnActivePlayerChanged);

----------------------------------------------------------------
-- Takeover activation, shared by the load-time path and the retry path.
----------------------------------------------------------------
local function ActivateViewportMode()
    bUseCameraView = true

    -- Drop anything the engine path drew; the viewport render replaces it.
    DestroyAllVisible()
    g_ViewSet = {}
    g_FilterFn = nil
    g_PersistSet = nil

    if OptionsManager and OptionsManager.GetYieldOn then
        bGlobalOn = OptionsManager.GetYieldOn() and true or false
    end

    LuaEvents.PlotEnterView.Add(OnPlotEnterView)
    LuaEvents.PlotLeaveView.Add(OnPlotLeaveView)

    LuaEvents.RespondViewPlots.Add(function(caller, plots)
        if caller ~= CALLER then
            return
        end

        for index in pairs(plots or {}) do
            g_ViewSet[index] = true
            if not g_VisibleSet[index] and ViewFilterAllows(index) then
                RenderPlot(index)
            end
        end
    end)

    Events.HexFOWStateChanged.Add(OnHexFogEvent)
    Events.RequestYieldDisplay.Add(OnRequestYieldDisplay)

    -- Another option changing must not rebuild the whole viewport
    Events.GameOptionsChanged.Add(function()
        if OptionsManager and OptionsManager.GetYieldOn then
            local bNew = OptionsManager.GetYieldOn() and true or false
            if bNew ~= bGlobalOn then
                bGlobalOn = bNew
                RenderCurrent()
            end
        end
    end)

    RequestAll()
end

-- A load-time refusal is often transient (the user interface may not be
-- ready yet), so retry on the first few game events before giving up for
-- good. The engine path below keeps drawing icons in the meantime.
local iDisableRetriesLeft = 2

local function RetryDisableEngineYieldIconManager()
    if bUseCameraView or not bHasCameraView or iDisableRetriesLeft <= 0 then
        return
    end

    iDisableRetriesLeft = iDisableRetriesLeft - 1

    if not (Exe and Exe.TryDisableEngineYieldIconManager) then
        return
    end

    local bOK, szReason = Exe.TryDisableEngineYieldIconManager()

    if bOK == true then
        bExeDisabled = true
        ActivateViewportMode()
    else
        print("YieldIconManager: engine YieldIconManager still not disabled (" .. tostring(szReason) .. ")")
    end
end

----------------------------------------------------------------
-- Event source: CameraView when the engine manager is off, otherwise
-- the engine's Events.ShowHexYield (unchanged legacy path).
----------------------------------------------------------------
if bUseCameraView then
    ActivateViewportMode()
else
    Events.ShowHexYield.Add(ShowHexYield);
    Events.GameplaySetActivePlayer.Add(RetryDisableEngineYieldIconManager)
end

----------------------------------------------------------------
-- Bit of a hack here, we want to ensure that the yield icons
-- are properly refreshed when starting a new game.
----------------------------------------------------------------
if not bUseCameraView then
    UI.RefreshYieldVisibleMode();
end
