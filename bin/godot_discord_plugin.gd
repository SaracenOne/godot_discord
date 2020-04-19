extends EditorPlugin
tool

var editor_interface : EditorInterface = null

func get_name() -> String:
	return "GodotDiscord"

func _enter_tree() -> void:
	editor_interface = get_editor_interface()
	
func _exit_tree() -> void:
	pass
