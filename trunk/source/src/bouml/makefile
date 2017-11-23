##START:version
#Butter version 1.2beta.175
##END:version
##START:date
#Thu Nov 23 23:30:27 2017
##END:date
##START:preamble
include $(ROOTDIR)/M_sys.mk

##END:preamble
##START:target bouml
BOUML_BOUMLSRC :=  \
	ClassGlobalCmd.cpp \
	CmdFamily.cpp \
	CppSettings.cpp \
	CppSettingsCmd.cpp \
	IdlSettings.cpp \
	IdlSettingsCmd.cpp \
	JavaSettings.cpp \
	JavaSettingsCmd.cpp \
	MiscGlobalCmd.cpp \
	OnInstanceCmd.cpp \
	PackageGlobalCmd.cpp \
	UmlActualParameter.cpp \
	UmlArtifact.cpp \
	UmlAttribute.cpp \
	UmlBaseActualParameter.cpp \
	UmlBaseArtifact.cpp \
	UmlBaseAttribute.cpp \
	UmlBaseClass.cpp \
	UmlBaseClassDiagram.cpp \
	UmlBaseClassItem.cpp \
	UmlBaseClassMember.cpp \
	UmlBaseClassView.cpp \
	UmlBaseCollaborationDiagram.cpp \
	UmlBaseComponent.cpp \
	UmlBaseComponentDiagram.cpp \
	UmlBaseComponentView.cpp \
	UmlBaseDeployment.cpp \
	UmlBaseDeploymentDiagram.cpp \
	UmlBaseDeploymentView.cpp \
	UmlBaseDiagram.cpp \
	UmlBaseExtraClassMember.cpp \
	UmlBaseFormalParameter.cpp \
	UmlBaseItem.cpp \
	UmlBaseNcRelation.cpp \
	UmlBaseNode.cpp \
	UmlBaseOperation.cpp \
	UmlBasePackage.cpp \
	UmlBaseRelation.cpp \
	UmlBaseSequenceDiagram.cpp \
	UmlBaseUseCase.cpp \
	UmlBaseUseCaseDiagram.cpp \
	UmlBaseUseCaseView.cpp \
	UmlBuiltin.cpp \
	UmlClass.cpp \
	UmlClassDiagram.cpp \
	UmlClassItem.cpp \
	UmlClassMember.cpp \
	UmlClassView.cpp \
	UmlCollaborationDiagram.cpp \
	UmlCom.cpp \
	UmlComponent.cpp \
	UmlComponentDiagram.cpp \
	UmlComponentView.cpp \
	UmlDeploymentDiagram.cpp \
	UmlDeploymentView.cpp \
	UmlDiagram.cpp \
	UmlExtraClassMember.cpp \
	UmlFormalParameter.cpp \
	UmlItem.cpp \
	UmlNcRelation.cpp \
	UmlNode.cpp \
	UmlOperation.cpp \
	UmlPackage.cpp \
	UmlParameter.cpp \
	UmlRelation.cpp \
	UmlSequenceDiagram.cpp \
	UmlSettings.cpp \
	UmlSettingsCmd.cpp \
	UmlStereotype.cpp \
	UmlTypeSpec.cpp \
	UmlUseCase.cpp \
	UmlUseCaseDiagram.cpp \
	UmlUseCaseView.cpp \
	aDirection.cpp \
	aRelationKind.cpp \
	aVisibility.cpp \
	anItemKind.cpp
BOUML_BOUML_DINC := $(filter %$(sufdep), $(BOUML_BOUMLSRC:.cpp=$(sufdep))) $(filter %$(sufdep), $(BOUML_BOUMLSRC:.c=$(sufdep)))
bouml$(suflib): LDFLAGS+= -L${QTDIR}/lib64 -lqt-mt -lm
bouml$(suflib) $(BOUML_BOUML_DINC) : CPPFLAGS+=-I${QTDIR}/include -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT 
BOUML_BOUML_OBJ+=$(filter %$(sufobj), $(BOUML_BOUMLSRC:.cpp=$(sufobj)))
USEROBJ+=$(BOUML_BOUML_OBJ)
bouml$(suflib) : $(BOUML_BOUML_SRC) $(BOUML_BOUML_DINC)
-include $(BOUML_BOUML_DINC)
$(eval $(call do_archive,bouml$(suflib),$(BOUML_BOUML_OBJ)))

all :: bouml$(suflib)
TARGETS+=bouml$(suflib)

$(eval $(call do_install,bouml$(suflib),LIB))

##END:target bouml
